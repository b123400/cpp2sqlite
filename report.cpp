//
//  report.cpp
//  cpp2sqlite
//
//  ©ywesee GmbH -- all rights reserved
//  License GPLv3.0 -- see License File
//  Created by Alex Bettarini on 25 Feb 2019
//

#define BOOST_LOG_DYN_LINK 1

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <ctime>
#include <libgen.h>     // for basename()

#include <boost/filesystem.hpp>
#include <boost/program_options.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/replace.hpp>

#include "report.hpp"

namespace REP
{
    std::ofstream ofs2;
    std::vector<std::string> ul;
    bool verboseFlag;

static void sanitize(std::string &msg)
{
    boost::replace_all(msg, "<", "&lt;");
    boost::replace_all(msg, ">", "&gt;");
}

static void toConsole(const std::string &msg)
{
    if (verboseFlag)
        std::cerr << msg << std::endl;
}

void init(std::string logDir,
          std::string filename,
          bool verbose)
{
    verboseFlag = verbose;

    std::string fullFilename(logDir);
    fullFilename += filename;

    ofs2.open(fullFilename.c_str());
    ofs2 << "<html>";
    ofs2 << "<head>";

    ofs2 << "<meta http-equiv=\"Content-Type\" content=\"text/html; charset=UTF-8\" />";
    ofs2 << "<style>";
    ofs2 << "body {font-family: verdana,arial,sans-serif; font-size: 13px;}";
    ofs2 << "h1 {text-align: center;}";
    ofs2 << "table { font-family: verdana,arial,sans-serif; font-size: 13px; color: #333333; border-width: 1px; border-color: #666666; border-collapse: collapse; width: 100% } ";
    ofs2 << "th { border-width: 1px; padding: 8px; border-style: solid; border-color: #666666; background-color: #dedede;} ";
    ofs2 << "td { border-width: 1px; padding: 8px; border-style: solid; border-color: #666666; background-color: #ffffff;}";
    ofs2 << "</style>";

    ofs2 << "</head>";
    ofs2 << "<body>" << std::endl;
}
    
void html_start_ul()
{
    ul.clear();
    ofs2 << "<ul>" << std::endl;
}

void html_end_ul()
{
    for (auto bullet : ul)
        ofs2 << bullet << std::endl;

    ofs2 << "</ul>" << std::endl;
}
    
void html_h1(const std::string &msg)
{
    toConsole(msg);

    std::string s(msg);
    sanitize(s);
    ofs2 << "<hr><h1>" << s << "</h1>" << std::endl;
}

void html_h2(const std::string &msg)
{
    toConsole(msg);

    std::string s(msg);
    sanitize(s);
    ofs2 << "<hr><h2>" << s << "</h2>" << std::endl;
}

void html_h3(const std::string &msg)
{
    toConsole(msg);

    std::string s(msg);
    sanitize(s);
    ofs2 << "<h3>" << s << "</h3>" << std::endl;
}

void html_h4(const std::string &msg)
{
    toConsole(msg);

    std::string s(msg);
    sanitize(s);
    ofs2 << "<h4>" << s << "</h4>" << std::endl;
}

void html_p(const std::string &msg)
{
    toConsole(msg);

    std::string s(msg);
    sanitize(s);
    ofs2 << "<p>" << s << "</p>" << std::endl;
}

void html_div(const std::string &msg)
{
    toConsole(msg);

    std::string s(msg);
    sanitize(s);
    ofs2 << "<div>" << s << "</div>" << std::endl;
}

void html_li(const std::string &msg)
{
    toConsole("• " + msg);

    std::string s(msg);
    sanitize(s);
    ul.push_back("<li>" + s + "</li>");
}

void terminate()
{
    // footer
    ofs2 << "<hr>\n";
    std::time_t seconds = std::time(nullptr);
    html_p(std::asctime(std::localtime( &seconds )));

    ofs2 << "</body>";
    ofs2 << "</html>";
    ofs2.close();
}

}
