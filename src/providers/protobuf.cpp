/*  ========================================================================================================================================
    Copyright (C) 2020 Eaton
    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.
    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.
    You should have received a copy of the GNU General Public License along
    with this program; if not, write to the Free Software Foundation, Inc.,
    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
    ========================================================================================================================================
*/

#include "pack/visitor.h"
#include <google/protobuf/descriptor.h>
#include <google/protobuf/descriptor.pb.h>
#include <google/protobuf/descriptor_database.h>
#include <google/protobuf/dynamic_message.h>
#include <iostream>

namespace pack {

namespace pb = google::protobuf;

template <Type ValType>
struct Convert
{
    using CppType       = typename ResolveType<ValType>::type;
    using WalkType      = std::tuple<pb::Message*, const pb::FieldDescriptor*>;
    using ConstWalkType = std::tuple<const pb::Message*, const pb::FieldDescriptor*>;

    static void decode(Value<ValType>& node, const ConstWalkType& proto)
    {
        auto refl = std::get<0>(proto)->GetReflection();

        if constexpr (ValType == Type::Bool) {
            node = refl->GetBool(*std::get<0>(proto), std::get<1>(proto));
        } else if constexpr (ValType == Type::Double) {
            node = refl->GetDouble(*std::get<0>(proto), std::get<1>(proto));
        } else if constexpr (ValType == Type::Float) {
            node = refl->GetFloat(*std::get<0>(proto), std::get<1>(proto));
        } else if constexpr (ValType == Type::Int32) {
            node = refl->GetInt32(*std::get<0>(proto), std::get<1>(proto));
        } else if constexpr (ValType == Type::Int64) {
            node = refl->GetInt64(*std::get<0>(proto), std::get<1>(proto));
        } else if constexpr (ValType == Type::String) {
            node = refl->GetString(*std::get<0>(proto), std::get<1>(proto));
        } else if constexpr (ValType == Type::UInt32) {
            node = refl->GetUInt32(*std::get<0>(proto), std::get<1>(proto));
        } else if constexpr (ValType == Type::UInt64) {
            node = refl->GetUInt64(*std::get<0>(proto), std::get<1>(proto));
        } else if constexpr (ValType == Type::UChar) {
            node = static_cast<unsigned char>(refl->GetUInt32(*std::get<0>(proto), std::get<1>(proto)));
        }
    }

    static void decode(ValueList<ValType>& node, const ConstWalkType& proto)
    {
        auto refl = std::get<0>(proto)->GetReflection();

        if constexpr (ValType == Type::UChar) {
            std::string str = refl->GetString(*std::get<0>(proto), std::get<1>(proto));
            node.setValue(typename ValueList<ValType>::ListType(str.begin(), str.end()));
        } else {
            for (int i = 0; i < refl->FieldSize(*std::get<0>(proto), std::get<1>(proto)); ++i) {
                if constexpr (ValType == Type::Bool) {
                    node.append(refl->GetRepeatedBool(*std::get<0>(proto), std::get<1>(proto), i));
                } else if constexpr (ValType == Type::Double) {
                    node.append(refl->GetRepeatedDouble(*std::get<0>(proto), std::get<1>(proto), i));
                } else if constexpr (ValType == Type::Float) {
                    node.append(refl->GetRepeatedFloat(*std::get<0>(proto), std::get<1>(proto), i));
                } else if constexpr (ValType == Type::Int32) {
                    node.append(refl->GetRepeatedInt32(*std::get<0>(proto), std::get<1>(proto), i));
                } else if constexpr (ValType == Type::Int64) {
                    node.append(refl->GetRepeatedInt64(*std::get<0>(proto), std::get<1>(proto), i));
                } else if constexpr (ValType == Type::String) {
                    node.append(refl->GetRepeatedString(*std::get<0>(proto), std::get<1>(proto), i));
                } else if constexpr (ValType == Type::UInt32) {
                    node.append(refl->GetRepeatedUInt32(*std::get<0>(proto), std::get<1>(proto), i));
                } else if constexpr (ValType == Type::UInt64) {
                    node.append(refl->GetRepeatedUInt64(*std::get<0>(proto), std::get<1>(proto), i));
                }
            }
        }
    }

    static void decode(ValueMap<ValType>& /*node*/, const ConstWalkType& /*proto*/)
    {
    }

    static void encode(const Value<ValType>& node, WalkType& proto)
    {
        auto refl = std::get<0>(proto)->GetReflection();

        if constexpr (ValType == Type::Bool) {
            refl->SetBool(std::get<0>(proto), std::get<1>(proto), node.value());
        } else if constexpr (ValType == Type::Double) {
            refl->SetDouble(std::get<0>(proto), std::get<1>(proto), node.value());
        } else if constexpr (ValType == Type::Float) {
            refl->SetFloat(std::get<0>(proto), std::get<1>(proto), node.value());
        } else if constexpr (ValType == Type::Int32) {
            refl->SetInt32(std::get<0>(proto), std::get<1>(proto), node.value());
        } else if constexpr (ValType == Type::Int64) {
            refl->SetInt64(std::get<0>(proto), std::get<1>(proto), node.value());
        } else if constexpr (ValType == Type::String) {
            refl->SetString(std::get<0>(proto), std::get<1>(proto), node.value());
        } else if constexpr (ValType == Type::UInt32) {
            refl->SetUInt32(std::get<0>(proto), std::get<1>(proto), node.value());
        } else if constexpr (ValType == Type::UInt64) {
            refl->SetUInt64(std::get<0>(proto), std::get<1>(proto), node.value());
        } else if constexpr (ValType == Type::UChar) {
            refl->SetUInt32(std::get<0>(proto), std::get<1>(proto), node.value());
        }
    }

    static void encode(const ValueList<ValType>& node, WalkType& proto)
    {
        auto refl = std::get<0>(proto)->GetReflection();

        if constexpr (ValType == Type::UChar) {
            std::string str(node.value().begin(), node.value().end());
            refl->SetString(std::get<0>(proto), std::get<1>(proto), str);
        } else {
            auto setFunc = [&](const auto& val) {
                if constexpr (ValType == Type::Bool) {
                    refl->AddBool(std::get<0>(proto), std::get<1>(proto), val);
                } else if constexpr (ValType == Type::Double) {
                    refl->AddDouble(std::get<0>(proto), std::get<1>(proto), val);
                } else if constexpr (ValType == Type::Float) {
                    refl->AddFloat(std::get<0>(proto), std::get<1>(proto), val);
                } else if constexpr (ValType == Type::Int32) {
                    refl->AddInt32(std::get<0>(proto), std::get<1>(proto), val);
                } else if constexpr (ValType == Type::Int64) {
                    refl->AddInt64(std::get<0>(proto), std::get<1>(proto), val);
                } else if constexpr (ValType == Type::String) {
                    refl->AddString(std::get<0>(proto), std::get<1>(proto), val);
                } else if constexpr (ValType == Type::UInt32) {
                    refl->AddUInt32(std::get<0>(proto), std::get<1>(proto), val);
                } else if constexpr (ValType == Type::UInt64) {
                    refl->AddUInt64(std::get<0>(proto), std::get<1>(proto), val);
                }
            };

            if constexpr (ValType == Type::Bool) {
                for (auto it : node) {
                    setFunc(it);
                }
            } else {
                for (const auto& it : node) {
                    setFunc(it);
                }
            }
        }
    }

    static void encode(const ValueMap<ValType>& /*node*/, WalkType& /*proto*/)
    {
    }
};


class ProtoSerializer : public Serialize<ProtoSerializer>
{
public:
    using WalkType = std::tuple<pb::Message*, const pb::FieldDescriptor*>;

    template <typename T>
    static void packValue(const T& val, WalkType& proto, Option opt)
    {
        if (val.hasValue() || fty::isSet(opt, Option::WithDefaults)) {
            Convert<T::ThisType>::encode(val, proto);
        }
    }

    static void packValue(const IObjectMap& /*val*/, WalkType& /*proto*/, Option /*opt*/)
    {
    }

    static void packValue(const IObjectList& val, WalkType& proto, Option opt)
    {
        auto refl = std::get<0>(proto)->GetReflection();
        for (int i = 0; i < val.size(); ++i) {
            const Attribute& node       = val.get(i);
            pb::Message*     child      = refl->AddMessage(std::get<0>(proto), std::get<1>(proto));
            auto             childProto = std::make_tuple(child, std::get<1>(proto));
            visit(node, childProto, opt);
        }
    }

    static void packValue(const INode& node, WalkType& proto, Option opt)
    {
        for (auto& it : node.fields()) {
            if (it->hasValue()) {
                auto fdesc = std::get<0>(proto)->GetDescriptor()->FindFieldByName(it->key());
                if (fdesc && fdesc->cpp_type() == pb::FieldDescriptor::CPPTYPE_MESSAGE && !fdesc->is_repeated()) {
                    auto refl  = std::get<0>(proto)->GetReflection();
                    auto child = WalkType(refl->MutableMessage(std::get<0>(proto), fdesc), fdesc);
                    visit(*it, child, opt);
                } else if (fdesc) {
                    auto child = WalkType(std::get<0>(proto), fdesc);
                    visit(*it, child, opt);
                } else {
                    throw std::runtime_error("Cannot find " + it->key());
                }
            }
        }
    }

    static void packValue(const IEnum& en, WalkType& proto, Option /*opt*/)
    {
        auto refl = std::get<0>(proto)->GetReflection();
        refl->SetEnumValue(std::get<0>(proto), std::get<1>(proto), en.asInt());
    }

    static void packValue(const IProtoMap& map, WalkType& proto, Option opt)
    {
        auto refl = std::get<0>(proto)->GetReflection();
        for (int i = 0; i < map.size(); ++i) {
            const INode& node       = map.get(i);
            pb::Message* child      = refl->AddMessage(std::get<0>(proto), std::get<1>(proto));
            auto         childProto = std::make_tuple(child, std::get<1>(proto));
            visit(node, childProto, opt);
        }
    }

    static void packValue(const IVariant& /*var*/, WalkType& /*proto*/, Option /*opt*/)
    {
    }
};

class ProtoDeserializer : public Deserialize<ProtoDeserializer>
{
public:
    using WalkType = std::tuple<const pb::Message*, const pb::FieldDescriptor*>;

    template <typename T>
    static void unpackValue(T& val, const WalkType& proto)
    {
        Convert<T::ThisType>::decode(val, proto);
    }

    static void unpackValue(IEnum& en, const WalkType& proto)
    {
        auto refl = std::get<0>(proto)->GetReflection();
        int  val  = refl->GetEnumValue(*std::get<0>(proto), std::get<1>(proto));
        en.fromInt(val);
    }

    static void unpackValue(IObjectMap& /*list*/, const WalkType& /*proto*/)
    {
    }

    static void unpackValue(IObjectList& list, const WalkType& proto)
    {
        auto refl = std::get<0>(proto)->GetReflection();

        for (int i = 0; i < refl->FieldSize(*std::get<0>(proto), std::get<1>(proto)); ++i) {
            const pb::Message& msg   = refl->GetRepeatedMessage(*std::get<0>(proto), std::get<1>(proto), i);
            auto&              obj   = list.create();
            auto               child = WalkType(&msg, nullptr);
            visit(obj, child);
        }
    }

    static void unpackValue(INode& node, const WalkType& proto)
    {
        for (auto& it : node.fields()) {
            auto fdesc = std::get<0>(proto)->GetDescriptor()->FindFieldByName(it->key());
            if (fdesc && fdesc->cpp_type() == pb::FieldDescriptor::CPPTYPE_MESSAGE && !fdesc->is_repeated()) {
                auto refl  = std::get<0>(proto)->GetReflection();
                auto child = WalkType(&refl->GetMessage(*std::get<0>(proto), fdesc), fdesc);
                visit(*it, child);
            } else if (fdesc) {
                auto child = WalkType(std::get<0>(proto), fdesc);
                visit(*it, child);
            }
        }
    }

    static void unpackValue(IProtoMap& map, const WalkType& proto)
    {
        auto refl = std::get<0>(proto)->GetReflection();

        for (int i = 0; i < refl->FieldSize(*std::get<0>(proto), std::get<1>(proto)); ++i) {
            const pb::Message& msg = refl->GetRepeatedMessage(*std::get<0>(proto), std::get<1>(proto), i);

            auto& obj   = map.create();
            auto  child = WalkType(&msg, nullptr);
            visit(obj, child);
        }
    }

    static void unpackValue(IVariant& /*var*/, const WalkType& /*proto*/)
    {
    }
};


namespace protobuf {

    static pb::Message* getMessage(const Attribute& attr)
    {
        if (const INode* node = dynamic_cast<const INode*>(&attr)) {
            static pb::SimpleDescriptorDatabase db;
            static pb::DescriptorPool           pool(&db);

            auto descr = pool.FindMessageTypeByName(node->protoName());
            if (!descr) {
                pb::FileDescriptorSet fs;
                fs.ParseFromString(node->fileDescriptor());
                for (int i = 0; i < fs.file().size(); ++i) {
                    if (!pool.FindFileByName(fs.file(i).name())) {
                        db.Add(fs.file(i));
                    }
                }
                descr = pool.FindMessageTypeByName(node->protoName());
            }

            if (!descr) {
                throw std::runtime_error("Cannot find description for " + node->protoName());
            }

            static pb::DynamicMessageFactory dmf(&pool);
            const pb::Message*               protoMsg = dmf.GetPrototype(descr);
            return protoMsg->New();
        }
        return nullptr;
    }

    fty::Expected<std::string> serialize(const Attribute& node, Option opt)
    {
        try {
            std::unique_ptr<pb::Message> msg(getMessage(node));

            auto proto = ProtoSerializer::WalkType(msg.get(), nullptr);
            ProtoSerializer::visit(node, proto, opt);

            return msg->SerializeAsString();
        } catch (google::protobuf::FatalException& ex) {
            return fty::unexpected(ex.message());
        } catch (std::exception& ex) {
            return fty::unexpected(ex.what());
        }
    }

    fty::Expected<void> deserialize(const std::string& content, Attribute& node)
    {
        try {
            std::unique_ptr<pb::Message> msg(getMessage(node));
            try {
                msg->ParseFromString(content);
            } catch (google::protobuf::FatalException& ex) {
                return fty::unexpected(ex.message());
            }

            auto proto = ProtoDeserializer::WalkType(msg.get(), nullptr);
            ProtoDeserializer::visit(node, proto);
            return {};
        } catch (const std::exception& e) {
            return fty::unexpected(e.what());
        }
    }

} // namespace protobuf

} // namespace pack
