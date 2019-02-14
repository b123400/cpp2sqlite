//
//  bag.hpp
//  cpp2sqlite
//
//  ©ywesee GmbH -- all rights reserved
//  License GPLv3.0 -- see License File
//  Created by Alex Bettarini on 23 Jan 2019
//

#ifndef bag_hpp
#define bag_hpp

#include <iostream>
#include <map>
#include "gtin.hpp"

namespace BAG
{
    struct packageFields {
        std::string shortName;
        std::string efp;
        std::string pp;
        std::vector<std::string> flags;
    };

    struct ItCode {
        std::string tindex;         // localized
        std::string application;    // localized
    };

    struct Pack {
        std::string description;
        std::string category;
        std::string gtin;
        std::string exFactoryPrice;
        std::string publicPrice;
        std::string limitationPoints;   // TODO
    };

    struct Preparation {
        std::string name;
        std::string description;
        std::string swissmedNo;     // same as regnr
        std::string orgen;
        std::string sb20;
        std::vector<Pack> packs;
        ItCode itCodes;
    };
    
    typedef std::vector<Preparation> PreparationList;
    typedef std::map<std::string, packageFields> PackageMap;

    void parseXML(const std::string &filename,
                  const std::string &language,
                  bool verbose);

    int getAdditionalNames(const std::string &rn,
                           std::set<std::string> &gtinUsed,
                           GTIN::oneFachinfoPackages &packages);

    std::string getPricesAndFlags(const std::string &gtin,
                                  const std::string &fromSwissmedic,
                                  const std::string &category="");

    std::vector<std::string> getGtinList();
    std::string getTindex(const std::string &rn);
    std::string getApplication(const std::string &rn);
    
    std::string formatPriceAsMoney(const std::string &price);

    packageFields getPackageFieldsByGtin(const std::string &gtin);

    void printStats();
}

#endif /* bag_hpp */
