#include "cryptoCrawler.h"

Currency currencies[100];

int main() {
    cout << endl;

    //Goes to https://coinmarketcap.com/ and outputs html to a text file
    getHTML();

    //Parses through the text file and makes the text easier to parse
    cleanFile();

    //Scrapes all the data for information of the top 100 currencies and saves the data to a Currency object
    mainScrape();

    cout << "Complete" << endl << endl << "    <Welcome to CryptoCrawler>" << endl << endl;

    //Menu System
    bool inMainMenu = true;
    char choiceOne;

    while (inMainMenu) {
        cout << "-------------------------------------" << endl << "\t     Main Menu" << endl << "-------------------------------------" << endl;
        cout << "    1. Single Currency Menu\n    2. Sort Cryptocurrencies\n    3. Market Average\n    4. Display All Cryptocurrencies\n    5. Exit\n";
        cout << "-------------------------------------" << endl << endl;
        cout << "Input: ";
        cin >> choiceOne;
        cout << endl;

        switch (choiceOne) {
            case '1':
            {
                Currency menuCurrency;
                string currencyInput;

                string currencyURL;
                string currencyName;

                cout << "Enter Currency: ";
                cin.ignore();
                getline(cin, currencyInput);

                currencyURL = currencyInput;
                currencyName = currencyInput;

                for(unsigned int i = 0; i < currencyInput.length(); i++){
                    if(currencyInput[i] == ' '){
                        currencyName.erase(i, 1);
                        currencyURL[i] = '-';
                    }
                    if(currencyInput[i] == '.'){
                        currencyURL = currencyInput.substr(0, i);
                    }
                }

                bool validInput = false;

                for (int i = 0; i < 100; i++) {
                    if (currencies[i].getName() == currencyName) {
                        menuCurrency = currencies[i];
                        validInput = true;
                        break;
                    }
                }

                if(!validInput){
                    cout << endl << "Error: \"" << currencyInput << "\" Not Found" << endl << endl;
                    break;
                }
                

                getHTMLthirtyDays(currencyURL);

                scrapeThirtyDays(menuCurrency);

                cout << "Complete" << endl << endl;

                //Second Menu`
                bool inSecondMenu = true;
                char choiceTwo;

                while (inSecondMenu) {
                    cout << "-------------------------------------" << endl << "\t    " << currencyInput << " Menu" << endl << "-------------------------------------" << endl;
                    cout << "    1. Display Information\n    2. Graph Historical Data\n    3. Price Prediction\n    4. Simulate Market\n    5. Return to Main Menu\n";
                    cout << "-------------------------------------" << endl << endl;
                    cout << "Input: ";
                    cin >> choiceTwo;
                    cout << endl;

                    switch (choiceTwo) {
                        case '1':
                        {
                            cout << fixed << setprecision(2) << right
                            << "-------------------------------------" << endl 
                            << setw(15) << currencyInput << " Information" << left << endl
                            << "-------------------------------------" << endl 
                            << setw(23) <<  "Price: " << setw(15) << menuCurrency.getPrice() << endl 
                            << setw(23) << "Market Cap: " << setw(15) << menuCurrency.getMarketCap() << endl 
                            << setw(23) << "Volume: " << setw(15) << menuCurrency.getVolume() << endl 
                            << setw(23) << "Circulating Supply: " << setw(15) << menuCurrency.getCirculatingSupply() << endl 
                            << setw(23) << "Percent Change: " << setw(15) << menuCurrency.getPercentChange() << endl
                            << "-------------------------------------" << endl << endl << endl;

                            break;
                        }

                        case '2':
                        {
                            displayGraph(menuCurrency);
                            break;
                        }

                        case '3':
                        {
                            currencyPrediction(menuCurrency, 0);
                            break;
                        }

                        case '4':
                        {
                            double moneyToInvest = 0; //the amount of money be invested

                            cout << "How much money would you like to invest?" << endl;
                            cout << "$";
                            cin >> moneyToInvest;

                            while (cin.fail() || moneyToInvest <= 0) { //if user inputted improper amount
                                cout << " " << endl;
                                cout << "Error: You must input a proper dollar amount" << std::endl;
                                cin.clear();
                                cin.ignore(1000, '\n');
                                cout << " " << endl;
                                cout << "How much money would you like to invest?" << endl;
                                cout << "$";
                                cin >> moneyToInvest;
                            }

                            currencyPrediction(menuCurrency, moneyToInvest);

                            break;

                        }
                        case '5':
                        {
                            inSecondMenu = false;
                            break;
                        }

                        default:
                        {
                            cout << "Invalid Input" << endl << endl;
                            break;
                        }
                    }
                }
                break;
            }

            case '2':
            {
                getTopTen();
                break;
            }

            case '3':
            {
                totalMarketChange();
                break;
            }

            case '4':
            {
                cout << endl << fixed;

                for (unsigned int i = 0; i < (sizeof(currencies) / sizeof(*currencies)); i++) {
                    cout << fixed << setprecision(2) 
                    << "-------------------------------------" << endl
                    << "\t \t" << (i+1) << endl << left 
                    << "-------------------------------------" << endl
                    << setw(23) << "Name: " << setw(15) << currencies[i].getName() << endl 
                    << setw(23) <<  "Price: " << setw(15) << currencies[i].getPrice() << endl 
                    << setw(23) << "Market Cap: " << setw(15) << currencies[i].getMarketCap() << endl 
                    << setw(23) << "Volume: " << setw(15) << currencies[i].getVolume() << endl 
                    << setw(23) << "Circulating Supply: " << setw(15) << currencies[i].getCirculatingSupply() << endl 
                    << setw(23) << "Percent Change: " << setw(15) << currencies[i].getPercentChange() << endl 
                    << "-------------------------------------" << endl << endl << endl;
                }

                cout << endl;
                break;
            }

            case '5':
            {
                cout << "Exiting" << endl << endl;
                return 0;
            }

            default:
            {
                cout << endl << "Invalid Input" << endl << endl;
            }
        }
    }
}

//Goes through the cleaned text file and extracts data
void mainScrape() {

    cout << "Extracting Data from HTML..." << endl;
    ifstream textFile("cleanCoinMarketCapHTML.txt");

    string fileString;

    int count = 0;
    bool wantNext = false;
    bool wantSecond = false;
    bool isName = false;
    bool isCap = false;
    bool isPrice = false;
    bool isVolume = false;
    bool isSupply = false;
    bool isChange = false;

    string tempName;

    double tempCap;
    double tempPrice;
    double tempVolume;
    double tempSupply;
    double tempChange;

    if (textFile.is_open()) {
        while (textFile.good()) {
            string foundString;
            textFile >> foundString;

            if (wantNext) {

                wantNext = false;
                string wantString;

                for (unsigned int i = 0; i < foundString.length(); i++) {
                    if (foundString.at(i) == '\"') {
                        wantString = foundString;

                        for (unsigned int j = 1; j < wantString.length(); j++) {
                            if (foundString.at(j + i) == '\"') {
                                wantString = wantString.substr(i + 1, j - 1);

                                break;
                            }
                        }
                        break;
                    }
                }

                if (isName) {

                    isName = false;
                    tempName = wantString;
                }

                else if (isCap) {

                    isCap = false;
                    tempCap = atof(wantString.c_str());
                }

                else if (isPrice) {

                    isPrice = false;
                    tempPrice = atof(wantString.c_str());
                }

                else if (isVolume) {

                    isVolume = false;
                    tempVolume = atof(wantString.c_str());
                }

                else if (isSupply) {

                    isSupply = false;
                    tempSupply = atof(wantString.c_str());
                }

                else if (isChange) {

                    isChange = false;
                    tempChange = atof(wantString.c_str());

                    currencies[count] = Currency(tempName, tempPrice, tempCap, tempVolume, tempSupply, tempChange);

                    tempName = "";
                    tempPrice = 0;
                    tempCap = 0;
                    tempVolume = 0;
                    tempSupply = 0;
                    tempChange = 0;
                    count++;
                }
            }

            if (wantSecond) {
                wantSecond = false;
                wantNext = true;
            }

            if (foundString == "currency-name\"") {
                wantNext = true;
                isName = true;
            }

            else if (foundString == "market-cap") {
                wantSecond = true;
                isCap = true;
            }

            else if (foundString ==  "class=\"price\"") {
                wantNext = true;
                isPrice = true;
            }

            else if (foundString == "class=\"volume\"") {
                wantNext = true;
                isVolume = true;
            }

            else if (foundString == "circulating-supply\"") {
                wantNext = true;
                isSupply = true;
            }

            else if (foundString == "data-timespan=\"24h\"") {
                wantNext = true;
                isChange = true;
            }
        }

        textFile.close();
    }
}

//Cleans the html text file for parsing
void cleanFile() {

    cout << "Cleaning Text File..." << endl;

    ifstream textFile("CoinMarketCapHTML.txt");
    string cleanString;

    if (textFile.is_open()) {
        while (textFile.good()) {
            string foundString;
            textFile >> foundString;
            cleanString += foundString + " ";
        }
    }

    textFile.close();

    //cleaning String
    for (unsigned int i = 0; i < cleanString.length(); i++) {
        if (isupper(cleanString[i]) && cleanString.at(i - 1) == ' ') {

            cleanString.erase(i - 1, 1);
        }
    }

    ofstream cleanWriteFile("cleanCoinMarketCapHTML.txt");

    if (cleanWriteFile.is_open()) {
        cleanWriteFile << cleanString;
    }

    cleanWriteFile.close();
}


//Get the price points for the past 30 days
void scrapeThirtyDays(Currency& currency) {

    cout << "Extracting Historical Data from HTML..." << endl;

    double thirtyDays[30];

    ifstream textFile("thirtyDays.txt");

    string fileString;
    int count = 0;
    int arrIndex = 0;
    bool wantNext = false;

    if (textFile.is_open()) {
        while (textFile.good()) {
            string foundString;
            textFile >> foundString;

            if (wantNext && (count - 1) % 4 == 0) {
                wantNext = false;
                string wantString;

                for (unsigned int i = 0; i < foundString.length(); i++) {
                    if (foundString.at(i) == '\"') {
                        wantString = foundString;

                        for (unsigned int j = 1; j < wantString.length(); j++) {
                            if (foundString.at(j + i) == '\"') {
                                wantString = wantString.substr(i + 1, j - 1);
                                break;
                            }
                        }
                        break;
                    }
                }

                thirtyDays[arrIndex] = atof(wantString.c_str());
                arrIndex++;
            }

            if (foundString == "data-format-fiat") {
                wantNext = true;
                count++;
            }
        }
    }

    currency.setThirtyDays(thirtyDays);
}

void getHTML()
{
    CURL *curl;
    FILE *fp;

    char const *url = "https://coinmarketcap.com/";

    cout << "Scraping https://coinmarketcap.com/ ..." << endl;

    char outfilename[FILENAME_MAX] = "CoinMarketCapHTML.txt";
    curl = curl_easy_init();

    if (curl)
    {
        fp = fopen(outfilename, "w");
        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
        curl_easy_perform(curl);
        curl_easy_cleanup(curl);
        fclose(fp);
    }
}

void getHTMLthirtyDays(string currency)
{
    CURL *curl;
    FILE *fp;
    string lowerCurrency;

    for (unsigned int i = 0; i < currency.length(); i++) {
        lowerCurrency += string(1, tolower(currency[i]));
    }

    string strUrl = "https://coinmarketcap.com/currencies/" + lowerCurrency + "/historical-data/";
    cout << endl << "Scraping " << strUrl.c_str() << " ..." << endl;

    char const *url = strUrl.c_str();
    char outfilename[FILENAME_MAX] = "thirtyDays.txt";
    curl = curl_easy_init();

    if (curl)
    {
        fp = fopen(outfilename, "w");
        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
        curl_easy_perform(curl);
        curl_easy_cleanup(curl);
        fclose(fp);
    }
}

void getTopTen() {
    bool validInput = false;
    map<double, string> cryptoMap;

    while (!validInput ) {
        int i = 0;
        int highRange;
        int lowRange;

        bool rangeInput = false;

        char tempInput;

        cout << "-------------------------------------" << endl << "\t     Sort Menu" << endl << "-------------------------------------" << endl;
        cout << "    1. Price\n    2. Market Cap\n    3. Volume\n    4. Cicurclating Supply\n    5. Percent Change\n";
        cout << "-------------------------------------" << endl << endl;
        cout << "Input: ";
        cin >> tempInput;
        cout << endl;

        while(!rangeInput){
            cout << "Low Range: ";
            cin >> lowRange;

            if(lowRange > 0 && lowRange <= 100){
                rangeInput = true;
            }
            else{
                cout << endl << "Invalid Input" << endl << endl;
            }
        }   

        rangeInput = false;

        while(!rangeInput){
            cout << "High Range: ";
            cin >> highRange;

            if(highRange >= lowRange && highRange <= 100){
                rangeInput = true;
            }
            else{
                cout << endl << "Invalid Input" << endl << endl;
            }
        }

        cout << endl;

        switch (tempInput) {
            case '1': {
                for (unsigned int i = 0; i < (sizeof(currencies) / sizeof(*currencies)); i++) {
                    cryptoMap.insert(pair<double, string>(currencies[i].getPrice(), currencies[i].getName()));
                }
                cout << "The Cryptocurrencies Sorted by Price From " << lowRange << " to " << highRange << " are: " << endl << endl;;

                for (auto iter = cryptoMap.rbegin(); iter != cryptoMap.rend(); ++iter) {
                    //cout << "i: " << i << endl;

                    if((i+1) <= highRange && (i+1) >= lowRange){
                        cout << left << setw(2) << (i + 1) << left << setw(4) << "." << left << setw(20) << iter->second << right << setw(10) << fixed << setprecision(2) << iter->first << "$" << endl;
                    }

                    i++;
                }

                cout << endl;
                validInput = true;
                break;
            }

            case '2': {
                for (unsigned int i = 0; i < (sizeof(currencies) / sizeof(*currencies)); i++) {
                    cryptoMap.insert(pair<double, string>(currencies[i].getMarketCap(), currencies[i].getName()));
                }

                cout << "The Cryptocurrencies Sorted by Market Cap From " << lowRange << " to " << highRange << " are: " << endl << endl;;

                for (auto iter = cryptoMap.rbegin(); iter != cryptoMap.rend(); ++iter) {

                    if((i+1) <= highRange && (i+1) >= lowRange){
                        cout << left << setw(2) << (i + 1) << left << setw(4) << "." << left << setw(25) << iter->second << right << setw(10) << fixed << setprecision(2) << iter->first << "$" << endl;
                    }

                    i++;
                }
                cout << endl;
                validInput = true;
                break;
            }

            case '3': {
                for (unsigned int i = 0; i < (sizeof(currencies) / sizeof(*currencies)); i++) {
                    cryptoMap.insert(pair<double, string>(currencies[i].getVolume(), currencies[i].getName()));
                }

                cout << "The Cryptocurrencies Sorted by Volume From " << lowRange << " to " << highRange << " are: " << endl << endl;;

                for (auto iter = cryptoMap.rbegin(); iter != cryptoMap.rend(); ++iter) {

                    if((i+1) <= highRange && (i+1) >= lowRange){
                        cout << left << setw(2) << (i + 1) << left << setw(4) << "." << left << setw(20) << iter->second << right << setw(10) << fixed << setprecision(2) << iter->first << endl;
                    }

                    i++;
                }
                cout << endl;
                validInput = true;
                break;
            }

            case '4': {
                for (unsigned int i = 0; i < (sizeof(currencies) / sizeof(*currencies)); i++) {
                    cryptoMap.insert(pair<double, string>(currencies[i].getCirculatingSupply(), currencies[i].getName()));
                }

                cout << "The Cryptocurrencies Sorted by Circulating Supply From " << lowRange << " to " << highRange << " are: " << endl << endl;;

                for (auto iter = cryptoMap.rbegin(); iter != cryptoMap.rend(); ++iter) {

                    if((i+1) <= highRange && (i+1) >= lowRange){
                        cout << left << setw(2) << (i + 1) << left << setw(4) << "." << left << setw(20) << iter->second << right << setw(10) << fixed << setprecision(2) << iter->first << endl;
                    }

                    i++;
                }
                cout << endl;
                validInput = true;
                break;
            }

            case '5': {
                for (unsigned int i = 0; i < (sizeof(currencies) / sizeof(*currencies)); i++) {
                    cryptoMap.insert(pair<double, string>(currencies[i].getPercentChange(), currencies[i].getName()));
                }

                cout << "The Cryptocurrencies Sorted by Percent Change From " << lowRange << " to " << highRange << " are: " << endl << endl;;

                for (auto iter = cryptoMap.rbegin(); iter != cryptoMap.rend(); ++iter) {

                    if((i+1) <= highRange && (i+1) >= lowRange){
                        cout << left << setw(2) << (i + 1) << left << setw(4) << "." << left << setw(20) << iter->second << right << setw(10) << fixed << setprecision(2) << iter->first << "%" << endl;
                    }

                    i++;
                }
                cout << endl;
                validInput = true;
                break;
            }

            default:
            {
                cout << "Invalid Input. Please enter a number between 1 and 5" << endl << endl;
                break;
            }
        }
    }
}

void currencyPrediction(Currency currency, double moneyToInvest) {
    double days[30]; //holds the prices for each day for the last 30 days
    double slopes[29]; //holds the day-to-day changes
    double average;
    double weightedAverage = 0;
    double thePrediction;

    int howManyDaysWorth = 0;

    for (int i = 0; i < 30; i++) {
        if (currency.getElementThirtyDays(i) > .000000001) {
            days[i] = currency.getElementThirtyDays(i);
            howManyDaysWorth++;
        }
    }

    for (int i = 0; i < howManyDaysWorth - 1; i++) {
        average = days[i] - days[i + 1];

        if (i < (howManyDaysWorth / 3)) { //first 1/3 of days
            slopes[i] = average * 3;
        } 

        else if (i < ((howManyDaysWorth * 2) / 3)) { //second 1/3 of days
            slopes[i] = average * 2;

        } 

        else if (i < howManyDaysWorth) { //last 1/3 of days
            slopes[i] = average;
        }

        else { //if days do not have a value
            slopes[i] = 0;
        }

        weightedAverage += slopes[i];
    }

    weightedAverage = weightedAverage / 6; //accounts for the multiplier given to more recent dates
    thePrediction = weightedAverage + days[0]; //adds projected change to most recent date

    if (moneyToInvest == 0) { //for prediction module
        cout << "The Predicted Price for the Following Day for ";
        cout << currency.getName();

        cout << " is $";
        cout << fixed;

        cout << setprecision(2);
        cout << thePrediction << endl;

        cout << " " << endl;
        cout << "The Most Recent Price for ";
        cout << currency.getName();

        cout << " is $";
        cout << fixed;
        cout << setprecision(2);
        cout << days[0];
        cout << " " << endl;

        cout << "The Price for ";
        cout << currency.getName();

        if (weightedAverage > 0) {
            cout << " is expected to increase by $";
        }

        else if (weightedAverage < 0) {
            cout << " is expected to decrease by $";
        }

        else {
            cout << " is expected to remain the same." << endl;
        }

        if (weightedAverage != 0) {
            cout << fixed;
            cout << setprecision(2);
            cout << weightedAverage << endl;
            cout << " " << endl;
        }
    }

    else { //for simulation module
        double predictedReturn; //the projected profit or loss

        predictedReturn = ((moneyToInvest * thePrediction) / days[0]) - moneyToInvest;
        cout << " " << endl;
        
        if (predictedReturn > 0) {
            cout << "Your investment in ";
            cout << currency.getName();
            cout << " is projected to create a profit of $";
            cout << fixed;

            cout << setprecision(2);
            cout << predictedReturn << endl;
        }
        else if (predictedReturn < 0) {
            cout << "Your investment in ";
            cout << currency.getName();
            cout << " is projected to cost you $";
            cout << fixed;

            cout << setprecision(2);
            cout << predictedReturn << endl;
        }
        else {
            cout << "Your investment in ";
            cout << currency.getName();
            cout << " is projected to neither gain nor lose you money." << endl;
        }
        cout << " " << endl;
    }
}

void displayGraph(Currency currency) {

    double highestPrice = -1;
    double lowestPrice = -1;
    //Declares an array of prices from the currency data
    //Defines array size variable for use in for loops
    double prices[30];

    //Sets each element of the prices array
    //equal to a price from getElementThirtyDays function
    int i = 0;
    for (int a = 29; a >= 0; a--) {
        //cout << fixed << a << ". " << currency.getElementThirtyDays(a) << endl;
        prices[i] = currency.getElementThirtyDays(a);
        i++;
    }
    //cout << endl;
    //Determines the highest price received within 30 days
    for (int a = 0; a < 30; a++) {
        if (lowestPrice == -1) {
            lowestPrice = prices[a];
        }
        if (prices[a] > highestPrice) {
            highestPrice = prices[a];
        }
        if (prices[a] < lowestPrice) {
            lowestPrice = prices[a];
        }
    }

    //cout << "Highest: " << highestPrice << endl << "Lowest: " << lowestPrice << endl << endl;

    char graph [30][20];

    double changeBetweenLines = (highestPrice - lowestPrice) / 20;

    //Creates Array
    double currentPriceRow = lowestPrice;

    for (int i = 0; i < 30; i++) {
        //cout << "Column " << i << endl << "Price: " << prices[i] << endl;

        for (int j = 0; j < 20; j++) {
            if (prices[i] <= currentPriceRow + changeBetweenLines && prices[i] >= currentPriceRow) {
                graph[i][j] = '-';
            }
            else {
                graph[i][j] = ' ';
            }
            currentPriceRow += changeBetweenLines;
        }
        currentPriceRow = lowestPrice;
    }

    //Prints out the graph of the currency
    cout << "  ---------------------------------------------------------------------------------------------------------------" << endl;
    cout << "                                                    <" << currency.getName() << " Graph>" << endl;
    cout << "  ---------------------------------------------------------------------------------------------------------------" << endl << endl << endl;

    cout << fixed << setprecision(2) << highestPrice << "$" << endl;
    for (int a = 19; a >= 0; a--) {
        cout << "        | ";
        for (int b = 0; b < 30; b++) {
            ;
            cout << graph[b][a] << "  ";
        }
        cout << endl;
    }
    cout << "        |___________________________________________________________________________________________________" << endl;
    cout << fixed << setprecision(2) << lowestPrice << "$                                            Past Thirty Days" << endl << endl << endl << endl;
}

//for loop the whole currency array and then add up all of the changes and then divide by that number of loops
void totalMarketChange(){

    int whatStatToSee;
    cout << "-------------------------------------" << endl
    << "\t    Average Menu" << endl
    << "-------------------------------------" << endl
    << "    1. Price\n    2. Market Cap\n    3. Volume\n    4. Circulating Supply\n    5. Percent Change\n"
    << "-------------------------------------" << endl << endl;
    cout << "What would you like to average?" << endl << "Input: ";
    cin >> whatStatToSee;
    
    while (cin.fail() || (whatStatToSee < 1) || (whatStatToSee > 5)) {
        cout << " " << endl;
        cout << "Invalid Input" << std::endl;
        cin.clear();
        cin.ignore(1000, '\n');
        cout << " " << endl;
        cout << "What statistic would you like to see the average for?" << endl;
        cin >> whatStatToSee;
    }
    
    cout << " " << endl;

    int size = 0;

    size = sizeof(currencies) / sizeof(currencies[0]);
    double price = 0;
    double marketCap = 0;
    double volume = 0;
    double circulatingSupply = 0;

    double change = 0;



    for (int x = 0; x < size; x ++){
        if (whatStatToSee == 1) {
            price += currencies[x].getPrice();
        }
        if (whatStatToSee == 2) {
            marketCap += currencies[x].getMarketCap();
        }
        if (whatStatToSee == 3) {
            volume += currencies[x].getVolume();
        }
        if (whatStatToSee == 4) {
            circulatingSupply += currencies[x].getCirculatingSupply();
        }
        if (whatStatToSee == 5) {
            change += currencies[x].getPercentChange();
        }
    }

    switch (whatStatToSee) {
        case 1:
        {
            cout << fixed << setprecision(2) << "Average Price: $" << price / size << endl; 
            break;
        }
        case 2:
        {
            cout << fixed << setprecision(0) << "Average Market Cap: $" << marketCap / size << endl;    
            break;
        }
        case 3:
        {
            cout << fixed << setprecision(0) << "Average Volume: $" << volume / size << endl;   
            break;
        }
        case 4:
        {
            cout << fixed << setprecision(0) << "Average Circulating Supply: " << circulatingSupply / size << " units" << endl; 
            break;
        }
        case 5:
        {
            cout << fixed << setprecision(2) << "Total Market Change: " << change / size << "%" << endl;
            break;
        }
    }
    cout << endl;
}

//Getter Methods for Currency Class
string Currency::getName() {
    return name;
}

double Currency::getPrice() {
    return price;
}

double Currency::getMarketCap() {
    return marketCap;
}

double Currency::getVolume() {
    return volume;
}

double Currency::getCirculatingSupply() {
    return circulatingSupply;
}

double Currency::getPercentChange() {
    return percentChange;
}

double Currency::getElementThirtyDays(int index) {
    return thirtyDays[index];
}

void Currency::setThirtyDays(double days[]) {
    for (int i = 0; i < 30; i++) {
        thirtyDays[i] = days[i];
    }
}