//define libcurl to be statically linked
#define CURL_STATICLIB

//include the external library libcurl
//Need to have libcurl installed for this to run
//also when compiling add -lcurl as a arguement
#include <curl/curl.h>



//standard includes
#include <iostream>

#include <fstream>

#include <string>

#include <cstring>

#include <cstdlib>

#include <iomanip>

#include <map>

//the cryptocurrency class
#include "Cryptocurrency.h"


//function defintions
void cleanFile();

void scrapeThirtyDays(Currency& currency);

void getHTMLthirtyDays(string currencyThirty);

void getHTML();

void mainScrape();

void currencyPrediction(Currency currency, double moneyToInvest);

void displayGraph(Currency currency);

void getTopTen();

void totalMarketChange();


using namespace std;