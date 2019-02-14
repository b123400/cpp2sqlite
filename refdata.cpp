//
//  refdata.cpp
//  cpp2sqlite
//
//  ©ywesee GmbH -- all rights reserved
//  License GPLv3.0 -- see License File
//  Created by Alex Bettarini on 21 Jan 2019
//

#include <set>
#include <libgen.h>     // for basename()
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/foreach.hpp>
#include <boost/algorithm/string.hpp>

#include "refdata.hpp"
#include "gtin.hpp"
#include "bag.hpp"
#include "swissmedic.hpp"
#include "beautify.hpp"

namespace pt = boost::property_tree;

namespace REFDATA
{

ArticleList artList;
int statsTotalGtinCount = 0;
    
void parseXML(const std::string &filename,
              const std::string &language)
{
    pt::ptree tree;
    const std::string nameTag = "NAME_" + boost::to_upper_copy( language );

    try {
        std::clog << std::endl << "Reading refdata XML" << std::endl;
        pt::read_xml(filename, tree);
    }
    catch (std::exception &e) {
        std::cerr << "Line: " << __LINE__ << " Error " << e.what() << std::endl;
    }
    
    std::clog << "Analyzing refdata" << std::endl;

    int statsArticleChildCount = 0;
    int statsItemCount = 0;
    try {
        BOOST_FOREACH(pt::ptree::value_type &v, tree.get_child("ARTICLE")) {
            statsArticleChildCount++;
            if (v.first == "ITEM")
            {
                statsItemCount++;

                std::string atype = v.second.get("ATYPE", "");
                if (atype != "PHARMA")
                    continue;

                std::string gtin = v.second.get<std::string>("GTIN", "");

                // Check that GTIN starts with 7680
                std::string gtinPrefix = gtin.substr(0,4); // pos, len
                if (gtinPrefix != "7680") // 76=med, 80=Switzerland
                    continue;
                
                GTIN::verifyGtin13Checksum(gtin);

                Article article;
                article.gtin_13 = gtin;
                article.gtin_5 = gtin.substr(4,5); // pos, len
                article.phar = v.second.get<std::string>("PHAR", "");
                article.name = v.second.get<std::string>(nameTag, "");
                BEAUTY::beautifyName(article.name);

                artList.push_back(article);
            }
//            else {
//                // one "<xmlattr>" and one "RESULT"
//                std::cout << " v.first: " << v.first << std::endl;
//            }
        }

        std::cout
        << "refdata PHARMA items with GTIN starting with \"7680\", count: " << artList.size()
        << " of " << statsItemCount
        //<< " (" << statsArticleChildCount << " articles)"
        << std::endl;
    }
    catch (std::exception &e) {
        std::cerr << basename((char *)__FILE__) << ":" << __LINE__ << ", Error " << e.what() << std::endl;
    }
}

// Each registration number can have multiple packages.
// Get all of them, one per line
// With the second argument we keep track of which GTINs have been used so far for this rn
// Return count added
int getNames(const std::string &rn,
             std::set<std::string> &gtinUsed,
             GTIN::oneFachinfoPackages &packages)
{
    int countAdded = 0;

    for (Article art : artList) {
        if (art.gtin_5 == rn) {
            countAdded++;
            statsTotalGtinCount++;
            
            std::string onePackageInfo;
#ifdef DEBUG_IDENTIFY_NAMES
            onePackageInfo += "ref+";
#endif
            onePackageInfo += art.name;

            std::string cat = SWISSMEDIC::getCategoryByGtin(art.gtin_13);
            std::string paf = BAG::getPricesAndFlags(art.gtin_13, "", cat);
            if (!paf.empty())
                onePackageInfo += paf;

            gtinUsed.insert(art.gtin_13);
            packages.gtin.push_back(art.gtin_13);
            packages.name.push_back(onePackageInfo);
        }
    }
    
    return countAdded;
}
    
bool findGtin(const std::string &gtin)
{
    for (Article art : artList)
        if (art.gtin_13 == gtin)
            return true;

    return false;
}

std::string getPharByGtin(const std::string &gtin)
{
    std::string phar;

    for (Article art : artList)
        if (art.gtin_13 == gtin) {
            phar = art.phar;
            break;
        }
    
    return phar;
}

void printStats()
{
    std::cout
    << "GTINs used from refdata " << statsTotalGtinCount
    << std::endl;
}

}
