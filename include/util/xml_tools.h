#ifndef __XML_TOOL_H_INCLUDED__
#define __XML_TOOL_H_INCLUDED__

#include<vector>
#include<boost/property_tree/ptree.hpp>
#include<boost/property_tree/xml_parser.hpp>

namespace util
{
	namespace xml
	{
		using namespace boost::property_tree;
		using namespace boost::property_tree::xml_parser;
		typedef ptree::const_iterator cptree_iter;
		typedef ptree::iterator ptree_iter;
		unsigned int node_count(const ptree& XML_data, const char* Node_name);
		void get_subtrees(const ptree& XML_data, const char* Node_name, std::vector<ptree>& Sub_trees);
		bool has_node(const ptree& XML_data, const char* Node_name);
		void prune_nodes(ptree& XML_data, const char* Node_name);

		namespace internal
		{
			void get_subtrees(const ptree& XML_data, const char* Node_name, std::vector<ptree>& Sub_trees);
		}
	}
}

unsigned int util::xml::node_count(const boost::property_tree::ptree& XML_data, const char* Node_name)
{
	unsigned int Result = 0;
	for (ptree::const_iterator Root_node = XML_data.begin(); Root_node != XML_data.end(); Root_node++)
	{
		Result += ((Root_node->first == Node_name) ? 1 : 0) + node_count(Root_node->second, Node_name);
	}
	return Result;
}

bool util::xml::has_node(const ptree& XML_data, const char* Node_name)
{
	for (cptree_iter Root = XML_data.begin(); Root != XML_data.end(); Root++)
	{
		if (Root->first == Node_name)
		{
			return true;
		}
		else
		{
			return has_node(Root->second, Node_name);
		}
	}
	return false;
}


void util::xml::prune_nodes(boost::property_tree::ptree& XML_data, const char* Node_name)
{
	for (ptree_iter Root_node = XML_data.begin(); Root_node != XML_data.end(); Root_node++)
	{
		Root_node->second.erase(Node_name);
		prune_nodes(Root_node->second, Node_name);
	}
}





#endif
