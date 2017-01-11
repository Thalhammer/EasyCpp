#include "XMLSerializer.h"
#include "../Bundle.h"
#include "../AnyArray.h"
#include <xml/rapidxml.hpp>
#include <xml/rapidxml_print.hpp>
#include <sstream>

namespace EasyCpp
{
	namespace Serialize
	{
		XMLSerializer::XMLSerializer()
		{
		}

		XMLSerializer::~XMLSerializer()
		{
		}

		std::string XMLSerializer::serialize(const AnyValue & a) const
		{
			using namespace rapidxml;

			std::function<void(xml_document<>*, xml_node<>*, const EasyCpp::Bundle&)> serialize_bundle;
			std::function<void(xml_document<>*, xml_node<>*, const std::string& name, const EasyCpp::AnyArray&)> serialize_array;

			serialize_bundle = [&serialize_bundle, &serialize_array](xml_document<>* doc, xml_node<>* node, const EasyCpp::Bundle& data) {
				for (auto& e : data) {
					if (e.first.substr(0, 1) == "-") {
						// Attribute
						std::string name = e.first.substr(1);
						std::string val = e.second.as<std::string>();
						node->append_attribute(doc->allocate_attribute(doc->allocate_string(name.data(), name.size()), doc->allocate_string(val.data(), val.size()), name.size(), val.size()));
					}
					else {
						if (e.first == "$value$") {
							std::string value = e.second.as<std::string>();
							node->value(doc->allocate_string(value.data(), value.size()), value.size());
						}
						else {
							// Element
							if (e.second.isConvertibleTo<EasyCpp::Bundle>()) {
								auto bundle = e.second.as<EasyCpp::Bundle>();
								xml_node<>* n = doc->allocate_node(node_element, doc->allocate_string(e.first.data(), e.first.size()), 0, e.first.size(), 0);
								node->append_node(n);
								serialize_bundle(doc, n, bundle);
							}
							else if (e.second.isConvertibleTo<EasyCpp::AnyArray>()) {
								serialize_array(doc, node, e.first, e.second.as<EasyCpp::AnyArray>());
							}
							else {
								std::string val = e.second.as<std::string>();
								node->append_node(doc->allocate_node(node_type::node_element, doc->allocate_string(e.first.data(), e.first.size()), doc->allocate_string(val.data(), val.size()), e.first.size(), val.size()));
							}
						}
					}
				}
			};
			serialize_array = [&serialize_bundle, &serialize_array](xml_document<>* doc, xml_node<>* node, const std::string& name, const EasyCpp::AnyArray& array) {
				auto* doc_name = doc->allocate_string(name.data(), name.size());
				for (auto& e : array) {
					if (e.isConvertibleTo<EasyCpp::Bundle>()) {
						auto bundle = e.as<EasyCpp::Bundle>();
						xml_node<>* n = doc->allocate_node(node_element, doc_name, 0, name.size(), 0);
						node->append_node(n);
						serialize_bundle(doc, n, bundle);
					}
					else if (e.isConvertibleTo<EasyCpp::AnyArray>()) {
						xml_node<>* n = doc->allocate_node(node_element, doc_name, 0, name.size(), 0);
						node->append_node(n);
						serialize_array(doc, n, name, e.as<EasyCpp::AnyArray>());
					}
					else {
						std::string val = e.as<std::string>();
						node->append_node(doc->allocate_node(node_type::node_element, doc_name, doc->allocate_string(val.data(), val.size()), name.size(), val.size()));
					}
				}
			};

			EasyCpp::Bundle data = a.as<EasyCpp::Bundle>();
			xml_document<> doc;
			{ // Create declaration entry
				xml_node<>* decl = doc.allocate_node(node_declaration);
				decl->append_attribute(doc.allocate_attribute("version", "1.0"));
				decl->append_attribute(doc.allocate_attribute("encoding", "utf-8"));
				doc.append_node(decl);
			}
			std::string root_name = "root";
			if (data.isSet("$xml_root_elem_name$"))
				root_name = data.get<std::string>("$xml_root_elem_name$");

			xml_node<>* root = doc.allocate_node(node_element, root_name.data(), 0, root_name.size(), 0);
			doc.append_node(root);
			serialize_bundle(&doc, root, data);
			std::stringstream ss;
			ss << doc;
			return ss.str();
		}

		AnyValue XMLSerializer::deserialize(const std::string & str)
		{
			using namespace rapidxml;
			xml_document<> doc;
			doc.parse<parse_non_destructive>((char*)str.c_str());

			std::function<EasyCpp::AnyValue(const xml_node<>*)> convert;
			convert = [&convert](const xml_node<>* node) -> EasyCpp::AnyValue {
				std::multimap<std::string, EasyCpp::AnyValue> buffer_array_check;
				const rapidxml::node_type t = node->type();
				if(t == rapidxml::node_element)
				{
					for (const rapidxml::xml_attribute<>* a = node->first_attribute()
						; a
						; a = a->next_attribute()
						) {
						buffer_array_check.insert({ "-" + std::string(a->name(), a->name_size()), std::string(a->value(), a->value_size()) });
					}

					for (const rapidxml::xml_node<>* n = node->first_node()
						; n
						; n = n->next_sibling()
						) {
						if (n->type() == node_data || n->type() == node_cdata) {
							buffer_array_check.insert({ "$value$", convert(n) });
							break;
						}
						else if (n->type() == node_element) {
							buffer_array_check.insert({ std::string(n->name(), n->name_size()), convert(n) });
						}
					}

					{
						// Check for duplicates and convert to array
						std::map<std::string, size_t> cnt;
						for (auto& e : buffer_array_check) {
							if (cnt.count(e.first) == 0)
								cnt[e.first] = 1;
							else cnt[e.first]++;
						}
						for (auto& e : cnt) {
							if (e.second > 1) {
								// Convert to array
								auto range = buffer_array_check.equal_range(e.first);
								EasyCpp::AnyArray array;
								for (auto it = range.first; it != range.second; it++) {
									array.push_back(it->second);
								}
								buffer_array_check.erase(e.first);
								buffer_array_check.insert({ e.first, array });
							}
						}
					}
					if (buffer_array_check.size() == 1 && buffer_array_check.begin()->first == "$value$")
						return buffer_array_check.begin()->second;
					EasyCpp::Bundle res;
					// Should no more contain duplicates
					for (auto& e : buffer_array_check) {
						res.set(e.first, e.second);
					}
					return res;
				}
				else if (t == rapidxml::node_data || t == rapidxml::node_cdata) {
					return std::string(node->value(), node->value_size());
				} else {
					throw std::logic_error("Invalid path called!");
				}
			};

			return convert(doc.first_node());
		}
	}
}
