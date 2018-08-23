#include <string>

using namespace std;

//The currency class is used to 
//store the data about a cryptocurrency
//It holds a currency's name, price,
//market cap, volume, circulatingSupply, and percent change

class Currency {
	private:

		//the private variables

		//store the currency's name
		string name;

		//store the currency's price
		double price;

		//store the currency's market cap
		double marketCap;

		//store the currency's volume
		double volume;

		//store the currency's circulating supply
		double circulatingSupply;

		//store the currency's percent change
		double percentChange;


		//store the currency's past thirty days pirce data
		double thirtyDays[30];

	public:

		//constructors

		//empty constructor
		Currency()
		{
			name = "";

			price = 0;

			marketCap = 0;

			volume = 0;

			circulatingSupply = 0;

			percentChange = 0;
		}

		//constructor to set data
		Currency(string name, double price, double marketCap, double volume, double circulatingSupply, double percentChange)
		{
			this->name = name;

			this->price = price;

			this->marketCap = marketCap;

			this->volume = volume;

			this->circulatingSupply = circulatingSupply;

			this->percentChange = percentChange;
		}

		//getter methods defintions

		string getName();

		double getPrice();

		double getMarketCap();

		double getVolume();

		double getCirculatingSupply();

		double getPercentChange();

		double getElementThirtyDays(int index);


		//setter method defintion
		
		void setThirtyDays(double days[]);
};