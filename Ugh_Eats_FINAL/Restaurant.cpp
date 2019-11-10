#include "Restaurant.h"
#include "Base.h"
#include "utils.h"

using namespace std;

string SEPARATOR = ";;;";

Product::Product()
{

}

Product::~Product()
{

}

void Product::parse(string str){
	vector<string> parts = utils::split(str, ':');

	name = parts.at(0);
	cuisine_type = parts.at(1);
	price = stoi(parts.at(2));
}

void Product::setName(string nm)
{
	name = nm;
}

void Product::setCuisineType(string ct)
{
	cuisine_type = ct;
}

void Product::setPrice(size_t p)
{
	price = p;
}

string Product::get_name() const
{
	return name;
}

string Product::get_cuisine_type() const
{
	return cuisine_type;
}

size_t Product::get_price() const
{
	return price;
}


ostream & operator<<(ostream & out, const Product & p)
{
	out << "   Name: " << p.name << endl;
	out << "   Cuisine Type: " << p.cuisine_type << endl;
	out << "   Price: " << p.price << endl;
	return out;
}


Restaurant::Restaurant()
{

}

Restaurant::~Restaurant()
{

}

void Restaurant::load(string path, Base * base){
	ifstream restaurants_text(path);

	string textline = "";
	vector<Restaurant*> restaurants_vec;
	while (getline(restaurants_text, textline)) {

		if (textline == SEPARATOR) getline(restaurants_text, textline);
		Restaurant restaurant;
		restaurant.setName(textline);

		getline(restaurants_text, textline);
		Address addr;
		addr.parse(textline);
		restaurant.setAddress(addr);

		vector<Product*> prods;

		while (getline(restaurants_text, textline)) {
			if (textline == ";;;") break;
			Product d;
			//cout << "Textline: " << textline << endl;
			d.parse(textline);

			Product * ptr2 = new Product;
			*ptr2 = d;
			//prods.push_back(&d);
			prods.push_back(ptr2);
			// cout << prods.at(0)->get_name() << endl;
		}
		restaurant.setProducts(prods);
		restaurant.setPriceAverage();

		Restaurant * ptr = new Restaurant;
		*ptr = restaurant;
		restaurants_vec.push_back(ptr);
	}
	//cout << "Vector size: " << restaurants_vec.at(0)->get_products().size() << endl;
	base->setRestaurants(restaurants_vec);
}

void Restaurant::setName(string nm){
	name = nm;
}

void Restaurant::setAddress(Address add)
{
	address = add;
}

void Restaurant::setCuisineTypes(vector <string> cts)
{
	cuisine_types = cts;
}

void Restaurant::setProducts(vector <Product*> ps)
{
	products = ps;
}

void Restaurant::setPriceAverage(size_t p)
{
	price_average = p;
}


void Restaurant::setPriceAverage()
{
	float total = 0;
	vector<Product*>::iterator it;
	for (it = products.begin(); it != products.end(); it++)
	{
		total += (*it)->get_price();
	}
	price_average = total / products.size();
}

void Restaurant::setBase(Base * b)
{
	base = b;
}

string Restaurant::get_name() const {
	return name;
}

Address Restaurant::get_address() const
{
	return address;
}

vector <string> Restaurant::get_cuisine_types() const
{
	return cuisine_types;

}

vector <Product*> Restaurant::get_products() const
{
	return products;
}

size_t Restaurant::get_price_average() const
{
	return price_average;
}

Base * Restaurant::get_base() const
{
	return base;
}

void Restaurant::update_price_average() //implementar depois
{

}

void Restaurant::print_price_average() //implementar depois - n�o seria necess�rio aqui um ostream ???
{

}

void Restaurant::update_cuisine_types() //implementar depois
{

}

bool Restaurant::add_product() //implementar depois
{
	return true;
}




ostream & operator<<(ostream & out, const Restaurant & r)
{
	out << "Name: " << r.name << endl;
	out << r.address;
	out << "Cuisine Types: ";
	if (r.cuisine_types.size() == 0)
		out << "none" << endl;
	else
	{
		for (auto i = 0; i < r.cuisine_types.size() - 1; i++)
		{
			out << r.cuisine_types[i] << ", ";
		}
		out << r.cuisine_types[r.cuisine_types.size() - 1] << endl;
	}
	out << "Price Average: " << r.price_average << endl;
	//out << "Number of Products:" << r.products.size() << endl << endl;


	/*
	out << "Products:" << endl;
	for (auto i = 0; i < r.products.size(); i++) {
		out << r.products.at(i) << endl;
	}*/
	out << "Number of Products: " << r.products.size() << endl << endl;


	return out;
}



bool operator<(Restaurant & left, Restaurant & right)
{
	return left.name < right.name;
}



Order::Order()
{

}

Order::~Order()
{

}

void Order::load(string path, Base * base){
	// cout << "Path: " << path << endl;
	ifstream orders_text(path);

	string textline;
	size_t temp; 
	vector<Order*> orders_vec;
	while(!orders_text.eof()){
		

		if (textline == SEPARATOR) {
			getline(orders_text, textline);
		}
		Order order;

	
		getline(orders_text, textline);
		temp = stoi(textline);		

		order.setID(temp);
		
		order.setBase(base);


		// orders_text >> textline;  //tem que detetar se e nullptr
		getline(orders_text, textline);
		//

		order.setRestaurant(base->findRestaurant(textline));
		
		// orders_text >> temp;
		getline(orders_text, textline);

		order.setDeliveryFee(temp);

		// orders_text >> textline;
		getline(orders_text, textline);

		order.setInsuccessMessage(textline);
		if (textline == "-") {
			order.setSuccess(true);
		}
		else {
			order.setSuccess(false);
		}

		getline(orders_text, textline);
		// orders_text >> textline;

		Time t; t.parse(textline);  // there was problem with what goes as textline in t.parse
		order.setTime(t);

		getline(orders_text, textline);
		// orders_text >> textline;

		Date d; d.parse(textline);
		order.setDate(d);

		vector<Product*> prods;
		while(getline(orders_text, textline)){
		// while(orders_text >> textline) {
			if (textline == ";;;") break;
			vector <string> parts = utils::split(textline, ':');

			Product p;
			p.setName(parts.at(0));
			p.setCuisineType(parts.at(1));
			p.setPrice(stoi(parts.at(2)));

			Product* ptr = new Product;
			*ptr = p;
			prods.push_back(ptr);
		}
		order.setProducts(prods);

		Order * ptr = new Order;
		*ptr = order;
		orders_vec.push_back(ptr);

		base->setOrders(orders_vec);
	}	
}

void Order::setID(size_t i)
{
	id = i;
}

void Order::setBase(Base * b)
{
	base = b;
}

void Order::setRestaurant(Restaurant* r)
{
	restaurant = r;
}

void Order::setClient(Client * c)
{
}

void Order::setProducts(vector <Product*> p)
{
	products = p;
}


void Order::setTime(Time t)
{
	time = t;
}

void Order::setDate(Date d){
	date = d;
}

void Order::setSuccess(bool s)
{
	success = s;
}

void Order::setInsuccessMessage(string message)
{
	insuccess_message = message;
}

void Order::setDeliveryFee(size_t fee)
{
	delivery_fee = fee;
}

size_t Order::getID() const
{
	return id;
}

Base * Order::getBase() const
{
	return base;
}

Restaurant* Order::getRestaurant() const
{
	return restaurant;
}

vector <Product*> Order::getProducts()  const
{
	return products;
}

Time Order::getTime() const
{
	return time;
}

Date Order::getDate() const
{
	return date;
}

bool Order::getSuccess() const
{
	return success;
}

string Order::getInsuccessMessage() const
{
	return insuccess_message;
}

size_t Order::getDeliveryFee() const
{
	return delivery_fee;
}
 
bool Order::calculate_fee() //implementar depois
{
	return true;
}

bool Order::calculate_time() //implementar depois
{
	return true;
}

ostream & operator<<(ostream & out, Order & o)
{
	out << "ID: " << o.id << endl;
	out << "Restaurant: " << o.restaurant->get_name() << endl;
	out << "Products: " << endl;
	vector<Product*>::iterator it;
	int c = 0;
	for (it = o.products.begin(); it != o.products.end(); it++)
	{
		out << *(*it);
		if (c != o.products.size() - 1)
			cout << endl;
		c++;
		
	}
	out << "Time: " << o.time << endl;
	out << "Date: " << o.date.str() << endl;
	if (o.success)
		out << "SUCCESSFUL" << endl;
	else
		out << o.insuccess_message << endl;
	out << "Delivery Fee: " << o.delivery_fee;
	return out;
}




