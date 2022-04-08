#!/usr/bin/python3
import getopt
import sys
import yaml

def traverse(_dict):
    obj = dict()
    for k in _dict.keys():
        v = _dict[k]
        obj[k] = traverse_inner(v)
    return obj

def traverse_inner(_dict):
    obj = dict()
    if 'enum' in _dict.keys():
        e_list = []
        if _dict['type'] == 'integer':
            if 'x-enumNames' in _dict.keys():
                for idx in range(len(_dict['x-enumNames'])):
                    e_list.append((_dict['x-enumNames'][idx], _dict['enum'][idx]))
        elif _dict['type'] == 'string':
            for idx in range(len(_dict['enum'])):
                e_list.append((_dict['enum'][idx], idx))
        return {'type' : 'enum', 'val' : e_list}
    if '$ref' in _dict.keys():
        return {'type' : 'ref', 'val' : _dict['$ref'].split('/')[-1]}
    if 'type' in _dict.keys():
        if _dict['type'] == 'boolean':
            return {'type' : "simple", 'val' : 'pack::Bool'}
        elif _dict['type'] == 'string':
            return {'type' : "simple", 'val' : 'pack::String'}
        elif _dict['type'] == 'integer':
            return {'type' : "simple", 'val' : 'pack::UInt32'}
        elif _dict['type'] == 'array':
            if '$ref' in _dict['items'].keys():
                return {'type' : "array", 'val' : f"{_dict['items']['$ref'].split('/')[-1]}"}
            elif 'type' in _dict['items'].keys():
                return {'type' : "array", 'val' : traverse_inner(_dict['items'])}
            return {'type' : "array", 'val' : {}}
        elif _dict['type'] == 'object':
            if 'properties' in _dict.keys():
                for k in _dict['properties']:
                    obj[k] = traverse_inner(_dict['properties'][k])
                return {'type' : 'obj', 'val' : obj}
            else:
                return {'type' : 'map', 'val' : ''}
    return {'type' : 'unknown', 'val' : ''}

def flatten_schemas(_schemas):
    for cl in list(_schemas):
        if _schemas[cl]['type'] == 'obj':
            attributes = _schemas[cl]['val']
            for attr in attributes:
                if attributes[attr]['type'] in ['obj']:
                    _schemas[f"{cl}-{attr}"] = {'type' : 'obj', 'val' : attributes[attr]['val']}
                    _schemas[cl]['val'][attr] = {'type' : 'ref', 'val' : f"{cl}-{attr}"}
    return _schemas


def parse_obj(_name, _schemas, _deps=False):
    deps = []
    cpp_class = ''
    ### class from object
    if _schemas[_name]['type'] == 'obj':
        cpp_class = f"""
struct {_name} : public pack::Node
{{"""
        attr_list = []
        attributes = _schemas[_name]['val']
        for attr in attributes:
            ### simple attributes
            if attributes[attr]['type'] == 'simple':
                cpp_class += f"""
    {attributes[attr]['val']} {attr} = FIELD("{attr}");"""
                attr_list.append(attr)
            ### ref attributes
            elif attributes[attr]['type'] == 'ref':
                objtp = attributes[attr]['val']
                if _schemas[objtp]['type'] == 'simple':
                    objtp = _schemas[objtp]['val']
                elif _schemas[objtp]['type'] == 'enum':
                    objtp = f"pack::Enum<{objtp}>"
                    deps.append(attributes[attr]['val'])
                else:
                    deps.append(attributes[attr]['val'])
                cpp_class += f"""
    {objtp} {attr} = FIELD("{attr}");"""
                attr_list.append(attr)
            ### array attributes
            elif attributes[attr]['type'] == 'array':
                arraytp = 'Array<None>'
                subcl = attributes[attr]['val']
                if subcl in _schemas.keys():
                    if _schemas[subcl]['type'] == 'simple':
                        if _schemas[subcl]['val'] == 'pack::String':
                            arraytp = 'pack::StringList'
                    elif _schemas[subcl]['type'] == 'obj':
                        arraytp = f"pack::ObjectList<{_schemas[subcl]['val']}>"
                else:
                    arraytp = f"pack::ObjectList<{_schemas[subcl]['val']}>"
                cpp_class += f"""
    {arraytp} {attr} = FIELD("{attr}");"""
                attr_list.append(attr)
            ### enum attributes
            elif attributes[attr]['type'] == 'enum':
                enumtp = 'pack::Enum<None>'
                subcl = attributes[attr]['val']
                if subcl in _schemas.keys():
                    enumtp = f"pack::Enum<{_schemas[subcl]['val']}>"
                cpp_class += f"""
    {enumtp} {attr} = FIELD("{attr}");"""
                attr_list.append(attr)
            ### unhandled attributes
            else:
                cpp_class += f"""
    // {attr}"""
                print(f"Warning! Attribute {attr} is a nested object, will be skipped")
                attr_list.append(f"/*{attr}*/")
        cpp_class += f"""

    using pack::Node::Node;
    META({_name}, {', '.join(attr_list)});
}};"""
    ### class from enum
    elif _schemas[_name]['type'] == 'enum':
        cpp_class = f"""
enum class {_name}
{{"""
        l = (',\n    ').join([f'{k} = {v}' for (k,v) in _schemas[_name]['val']])
        cpp_class += f"""
    {l}
}};"""
    else:
        print(f"Warning! Object {_name} is not supported")
    print(cpp_class)

    if _deps:
        for d in set(deps):
            print("\n// =============================================================================")
            parse_obj(d, _schemas, _deps)

def usage():
    print("Convert Swagger yaml schemas to pack::Node classes")
    print("Usage:")
    print("./converter_to_pack.py [options] <yaml_file> [class]")
    print("<yaml_file> : path to the yaml file")
    print("[class]     : optionally specify one single class to convert")
    print("Options:")
    print("\t-h/--help   : print this help")
    print("\t-d/--deps   : recursively parse a schema and all its dependencies")

def main():
    try:
        opts, args = getopt.getopt(sys.argv[1:], "hd", ["help", "deps"])
    except getopt.GetoptError as err:
        # print help information and exit:
        print(err)
        usage()
        sys.exit(1)

    parse_deps = False

    for opt, val in opts:
        if opt in ("-h", "--help"):
            usage()
            sys.exit()
        elif opt in ("-d", "--deps"):
            parse_deps = True
        else:
            print(f"Invalif option {opt}")

    if len(args) < 1:
        print("No yaml file specified")
        usage()
        sys.exit(1)

    yaml_file = args[0]    

    try:
        fp = open(yaml_file)
        yaml_struct = yaml.load(fp, Loader=yaml.FullLoader)

        schemas = traverse(yaml_struct['components']['schemas'])
        fl_schemas = flatten_schemas(schemas)

        ### if no schema is requested, convert all
        objs = schemas.keys() if len(args) == 1 else [args[1]]

        for cl in objs:
            parse_obj(cl, fl_schemas, parse_deps)

    except Exception as err:
        print(err)
        usage()
        sys.exit(1)

if __name__ == "__main__":
    main()
