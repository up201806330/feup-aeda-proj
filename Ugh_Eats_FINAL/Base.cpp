#include "Base.h"
#include "utils.h"
using namespace std;

#include <set>
#include <list>
#include <iomanip>

Base::Base()
{

}

Base::~Base()
{

}

tuple<string, vector<string> > Base::parseAreaOfInfluence(string str) {

	// ':' separates the "name" of the base to the rest 
	vector<string> parts = utils::split(str, ':');
	for (auto & part : parts) utils::trim(part);

	vector<string> towns = utils::split(parts.at(1), ';');
	for (auto & town : towns) utils::trim(town);

	return make_tuple(parts.at(0), towns);
}

vector<Base*> Base::load(string path){

	Base::load_blacklist();

	ifstream base_text(path);

	if (!base_text.is_open()) cout << "Error reading";

	vector<Base*> bases;
	string textline;

	vector<string> areaOfInfluence;

	while(!base_text.eof()){
		Base base;
		getline(base_text, textline);
		
		tuple<string, vector<string> > distAndAreaOfInf = Base::parseAreaOfInfluence(textline);
		base.setDistrict(get<0>(distAndAreaOfInf));
		base.setAreaOfInfluence(get<1>(distAndAreaOfInf));
		getline(base_text,textline);
		Address addr;
		addr.parse(textline);
		base.setAddress(addr);

		getline(base_text, textline);
		Restaurant::load(textline, &base);

		getline(base_text, textline);
		Order::load(textline, &base);

		/*
		for (auto & x : base.getOrders()) {
			cout << "ID: " << x->getID() << endl;
		}
		*/

		getline(base_text, textline);
		Worker::load(textline, &base);

		/*
		for (int i = 0; i < base.getWorkers().size(); i++) {
			cout << "Name: " << base.getWorkers().at(i)->get_name() << endl;
		}
		*/

		base.setAdmin(dynamic_cast<Admin*>(base.getWorkers().at(0)));
		 
		getline(base_text, textline);
		Client::load(textline, &base);
		// cout << base.getClients().at(0)->get_base()->getAreaOfInfluence().size() << endl;
		getline(base_text, textline);

		Base * ptr = new Base;
		*ptr = base;
		bases.push_back(ptr);
	}
	base_text.close();

	return bases;
}

vector<string> Base::blacklist = {};

void Base::load_blacklist()
{
	ifstream stream("blacklisted.txt"); 

	if (!stream.is_open()) cout << "Error reading";

	string input;

	while (!stream.eof()) {
		getline(stream, input);
		blacklist.push_back(input);
	}

	stream.close();
}

Restaurant * Base::findRestaurant(string str){
	for (auto & restaurant : restaurants)
		if (restaurant->get_name() == str)
			return restaurant;
	return nullptr;
}

map<int, Order*> Base::findOrders(string str) {
	
	map<int, Order*> result = {};

	// in case there are no orders, the textline is "-" and therefore the map should have 0 elements
	if (str == "-") {
		return result;
	}

	vector<string> ids = utils::split(str, ':');
	// cout << "Size: " << ids.size() << endl;
	for (auto i = 0 ; i < ids.size() ; i++) {
		int x = stoi(ids.at(i));
		// cout << "x: " << x << endl;
		
		map<int, Order*>::iterator it = orders.find(x);

		if (it != orders.end()) {
			// cout << "x: " << x << " second: " << (*it).second << " ." << endl;
			result.insert(pair<int, Order*>(x, (*it).second));
		}
	}
	return result;
}


void Base::setRestaurantsFileName(string str) {
	restaurantsFileName = str;
}

void Base::setDeliveriesFileName(string str) {
	deliveriesFileName = str;
}

void Base::setWorkersFileName(string str) {
	workersFileName = str;
}

void Base::setClientsFileName(string str) {
	clientsFileName = str;
}


void Base::setDistrict(string d){
	district = d;
}

void Base::setAreaOfInfluence(vector<string> areaOfInf) {
	areaOfInfluence = areaOfInf;
}

void Base::setAddress(Address add){
	address = add;
}

void Base::setWorkers(vector<Worker*> workers){
	this->workers = workers;
}

void Base::setAdmin(Admin * administrador){
	admin = administrador;
}

void Base::setClients(vector<Client*> clients){
	this->clients = clients;
}

void Base::setOrders(map<int, Order*> orders)
{
	this->orders = orders;
}

void Base::setRestaurants(vector<Restaurant*> restaurants){
	this->restaurants = restaurants;
}



string Base::getRestaurantsFileName() const {
	return restaurantsFileName;
}

string Base::getDeliveriesFileName() const {
	return deliveriesFileName;
}

string Base::getWorkersFileName() const {
	return workersFileName;
}

string Base::getClientsFileName() const {
	return clientsFileName;
}





string Base::getDistrict() const{
	return district;
}

vector<string> Base::getAreaOfInfluence() const {
	return areaOfInfluence;
}

Address Base::getAddress() const{
	return address;
}

const vector<Worker*> & Base::getWorkers() const{
	return workers;
}

Admin * Base::getAdmin() const{
	return admin;
}

const vector<Client*> & Base::getClients() const{
	return clients;
}

const vector<Restaurant*> & Base::getRestaurants() const{
	return restaurants;
}

const map<int, Order*> & Base::getOrders() const{
	return orders;
}

//Visualiza��o de Informa��o
void Base::seeAllClients()
{
	cout << "ALL CLIENTS" << endl << endl;
	vector<Client*>::iterator it;
	for (it = clients.begin(); it != clients.end(); it++)
	{
		(*it)->print();
		cout << endl << endl;
	}
}

void Base::seeOneClient()
{
	cout << "Pick the Client you want to see: " << endl << endl;
	vector<Client*>::iterator it;
	int id = 1;
	string answer;
	bool retry = true;
	do
	{
		try
		{
			for (it = clients.begin(); it != clients.end(); it++)
			{
				cout << id << ". " << (*it)->get_name() << endl;
				id++;
			}
			cout << "0. Go Back" << endl;
			cout << ">> ";
			getline(cin, answer);
			if (answer == "0")
			{
				cin.clear();
				utils::clear_screen();
				return;
			}
			if (!isNumber(answer))
				throw InvalidNumberException(answer);
			int input = stoi(answer);
			retry = false;
			if (InvalidOptions(clients.size(), input))
				throw InvalidOptionException(input);
		}
		catch (InvalidOptionException & o)
		{
			retry = true;
			cout << o;
			cout << "Try Again!" << endl << endl;
			id = 1;
		}
		catch (InvalidNumberException & s)
		{
			retry = true;
			cout << s;
			cout << "Try Again!" << endl << endl;
			id = 1;
		}
	}while(retry);
	

		cout << endl << "INFO" << endl;
		clients.at(stoi(answer) - 1)->print();
		
		cout << "\n>> ";
		cin.ignore();
}

void Base::seeAllRestaurants()
{
	cout << "ALL RESTAURANTS" << endl << endl;
	vector<Restaurant*>::iterator it;
	for (it = restaurants.begin(); it != restaurants.end(); it++)
	{
		cout << *(*it);
	}
}

void Base::seeOneRestaurant()
{
	cout << "Pick the Restaurant you want to see" << endl << endl;
	vector<Restaurant*>::iterator it;
	int id = 1;
	string answer;
	bool retry = true;
	do
	{
		try
		{
			retry = false;
			for (it = restaurants.begin(); it != restaurants.end(); it++)
			{
				cout << id << ". " << (*it)->get_name() << endl;
				id++;
			}
			cout << "0. Go Back" << endl;
			cout << ">> ";

			getline(cin, answer);
			if (answer == "0")
			{
				cin.clear();
				utils::clear_screen();
				return;
			}
			if (!isNumber(answer))
				throw InvalidNumberException(answer);
			int input = stoi(answer);
			if (InvalidOptions(restaurants.size(), input))
				throw InvalidOptionException(input);
		}
		catch (InvalidOptionException & o)
		{
			retry = true;
			cout << o;
			cout << "Try Again!" << endl << endl;
			id = 1;
		}
		catch (InvalidNumberException & s)
		{
			retry = true;
			cout << s;
			cout << "Try Again!" << endl << endl;
			id = 1;
		}

	} while (retry);



	int input = stoi(answer);
		cout << endl << "INFO" << endl;
		cout << (*restaurants.at(input - 1));
		if ((*restaurants.at(input - 1)).get_products().size() == 0)
			cout << endl;
		else
		{
			cout << "Products:" << endl;
			vector<Product*>::iterator ite;

			// REVERTED CHANGES HERE
			vector<Product*> vec = restaurants.at(input - 1)->get_products();
			for (ite = vec.begin(); ite != vec.end(); ite++)
				// for (ite = (*restaurants.at(answer - 1)).get_products().begin(); ite != (*restaurants.at(answer - 1)).get_products().end(); ite++)
			{
				cout << *(*ite) << endl;
			}

		}

		cout << "\n>> ";
		cin.ignore();
}


void Base::seeAllWorkers()
{

	cout << "ALL WORKERS" << endl << endl;
	cout << "Administrators:" << endl << endl;
	for (int i = 0; i < workers.size(); i++)
	{
		Admin *a = dynamic_cast<Admin *> (workers[i]);
		if (a != NULL)
		{
			a->print();
			cout << endl;
		}
			
		else
			continue;
		
	}
	cout << endl << "Deliverers: " << endl << endl;

	for (int j = 0; j < workers.size(); j++)
	{
		Delivery *d = dynamic_cast<Delivery *> (workers[j]);
		if (d != NULL)
		{
			d->print();
			cout << endl;

		}
			
		else
			continue;
	}
	
}

void Base::seeOneWorker()
{
	cout << "Pick the worker you want to see" << endl;
	vector<Worker*>::iterator it;
	int id = 1, answer;
	string input;
	bool retry = true;
	do
	{
		try
		{
			for (it = workers.begin(); it != workers.end(); it++)
			{
				cout << id << ". " << (*it)->get_name() << endl;
				id++;
			}

			cout << "0. Go Back" << endl;
			cout << ">> ";
			getline(cin, input);
			if (input == "0")
			{
				cin.clear();
				utils::clear_screen();
				return;
			}
			if (!isNumber(input))
				throw InvalidNumberException(input);
			answer = stoi(input);
			retry = false;
			if (InvalidOptions(workers.size(), answer))
				throw InvalidOptionException(answer);
		}
		catch (InvalidOptionException & o)
		{
			retry = true;
			cout << o;
			cout << "Try Again!" << endl << endl;
			id = 1;
		}
		catch (InvalidNumberException & s)
		{
			retry = true;
			cout << s;
			cout << "Try Again!" << endl << endl;
			id = 1;
		}

	} while (retry);
	
		cout << "\nINFO" << endl;
		Admin *a = dynamic_cast<Admin *> (workers.at(answer - 1));
		if (a != NULL) //good practice ?
		{
			cout << "Administrator" << endl;
		}
		else
			cout << "Deliverer" << endl;
		workers.at(answer - 1)->print();
		
		cout << "\n>> ";
		cin.ignore();
	

}

void Base::seeAllDeliverers()
{
	cout << "ALL DELIVERERS" << endl << endl;
	for (int i = 0; i < workers.size(); i++)
	{
		Delivery *d = dynamic_cast<Delivery *> (workers[i]);
		if (d != NULL)
		{
			d->print();
			cout << endl;
		}

	}
}

void Base::seeAllAdministrators()
{
	cout << "ALL ADMINISTRATORS" << endl << endl;
	for (int i = 0; i < workers.size(); i++)
	{
		Admin *a = dynamic_cast<Admin*> (workers[i]);
		if (a != NULL)
		{
			a->print();
			cout << endl;
		}
	}
}

void Base::seeAllOrders()
{
	cout << "ALL ORDERS" << endl << endl;
	map<int, Order*>::iterator it;
	for (it = orders.begin(); it != orders.end(); it++)
	{
		cout << *((*it).second) << endl << "-------------------------------" << endl;
	}

	cout << "\n>> ";
	cin.ignore();
}

void Base::seeOneOrder()
{
	bool retry = true;
	do
	{
		try
		{
			retry = false;
			cout << "Pick the Order you want to see" << endl;
			int op = 1, answer;
			string input;
			map<int, Order*>::iterator it;
			vector<int> temporary_ids;

			for (it = orders.begin(); it != orders.end(); it++)
			{
				cout << op << ". ID: " << (*it).second->getID();
				cout << endl;
				temporary_ids.push_back((*it).second->getID());
			}
			cout << "0. Go Back" << endl;
			cout << ">> ";
			getline(cin, input);
			if (input == "0")
			{
				cin.clear();
				utils::clear_screen();
				return;
			}
			if (!isNumber(input))
				throw InvalidNumberException(input);
			answer = stoi(input);
			if (InvalidOptions(orders.size(), answer))
				throw InvalidOptionException(answer);
			cout << "INFO" << endl;
			cout << *(orders.at(temporary_ids.at(answer - 1)));

			cout << "\n>> ";
			cin.ignore();
		}
		catch (InvalidNumberException & n)
		{
			retry = true;
			cout << n;
			cout << "Try Again!" << endl << endl;
		}
		catch (InvalidOptionException & o)
		{
			retry = true;
			cout << o;
			cout << "Try Again!" << endl << endl;
		}
	} while (retry);
	
}


void Base::seeProfits()
{
	cout << "Profit for this Base: ";
	map<int, Order*>::iterator it;
	double total = 0;
	for (it = orders.begin(); it != orders.end(); it++)
	{
		total += (*it).second->getDeliveryFee();
	}
	cout << total << endl;

	cout << "\n>> ";
	cin.ignore();
}

void Base::seeProfitsPerRestaurant()
{
	cout << "Profits per Restaurant" << endl << endl;
	vector< Restaurant *>::iterator it;
	for (it = restaurants.begin(); it != restaurants.end(); it++)
	{
		double total = 0;
		map<int, Order*>::iterator ite;
		for (ite = orders.begin(); ite != orders.end(); ite++)
		{
			if ((*it)->get_name() == (*ite).second->getRestaurant()->get_name())
			{
				total += (*ite).second->getDeliveryFee();
			}
		}
		cout << (*it)->get_name() << ": " << total << endl;
	}

	cout << "\n>> ";
	cin.ignore();

}

void Base::seeProfitsPerClient()
{
	cout << "Profits per Client" << endl << endl;
	vector<Client*>::iterator it;
	for (it = clients.begin(); it != clients.end(); it++)
	{
		double total = 0;
		map<int, Order*> vec = (*it)->get_orders();
		map<int, Order*>::iterator ite;
		for (ite = vec.begin(); ite != vec.end(); ite++)
		{
			total += (*ite).second->getDeliveryFee();
		}
		cout << (*it)->get_name() << ": " << total << endl;
	}

	cout << "\n>> ";
	cin.ignore();
}

void Base::seeProfitsPerTime()
{

	Date_time left, right, null;
	string left_string, right_string;
	bool retry = true;
	bool teste;
	bool re = true;
	do {
		re = false;
		try {
			do
			{
				try
				{
					retry = false;
					cout << "Please input starting date (day:month:year): "; getline(cin, left_string);
					teste = left.parse(left_string);
					if (!teste || !isDateValid(left)) {
						throw InvalidDateException(left_string);
					}
				}
				catch (InvalidDateException & d)
				{
					retry = true;
					cout << d;
					cout << "Try Again!" << endl << endl;
				}

			} while (retry);


			retry = true;
			do
			{
				try
				{
					retry = false;
					cout << endl << "Please input ending date (day:month:year): "; getline(cin, right_string);
					teste = right.parse(right_string);
					if (!teste || !isDateValid(right)) {
						throw InvalidDateException(right_string);
					}
				}
				catch (InvalidDateException & d)
				{
					retry = true;
					cout << d;
					cout << "Try Again!" << endl << endl;
				}
			} while (retry);

			if (right < left)
				throw InvalidDatesException(left_string, right_string);
		}
		catch (InvalidDatesException & d)
		{
			re = true;
			cout << d;
			cout << "Try Again!" << endl << endl;
		}
		
	} while (re);

	

	double total = 0;
	map<int, Order*>::iterator it;
	for (it = orders.begin(); it != orders.end(); it++) {
		if (left < (*it).second->getDateTime() && (*it).second->getDeliver()->getDateTime() < right)
			total += (*it).second->getDeliveryFee();
	}

	cout << "Profits in set period: " << total << endl;

	cout << "\n>>";
	cin.ignore();

	/*
	string datei, datef, houri, hourf;
	cout << "Type in the initial date (dd/mm/yyyy)" << endl;
	getline(cin, datei);
	if (cin.eof())
	{
		cin.clear();
		return;
	}
	Date date1;
	date1.parse(datei);
	cout << "Type in the initial hours (hh:mm:ss)" << endl;
	getline(cin, houri);
	if (cin.eof())
	{
		cin.clear();
		return;
	}
	Time hour1;
	hour1.parse(houri);
	cout << "Type in the final date (dd/mm/yyyy)" << endl;
	getline(cin, datef);
	if (cin.eof())
	{
		cin.clear();
		return;
	}
	Date date2;
	date2.parse(datef);
	cout << "Type in the final hours (hh:mm:ss)" << endl;
	getline(cin, hourf);
	if (cin.eof())
	{
		cin.clear();
		return;
	}
	Time hour2;
	hour2.parse(hourf);
	double total = 0;
	map<int, Order*>::iterator it;
	for (it = orders.begin(); it != orders.end(); it++)
	{
		if (date1 < (*it).second->getDate()) // se a data for depois da data do início
		{
			if ((*it).second->getDate() < date2) // a data for antes da data do fim
				total += (*it).second->getDeliveryFee();
			else if (date2 < (*it).second->getDate())  // a data for depois da data do fim
				continue;
			else // a data for igual à data do fim -> testar horas
			{
				if (hour2 < (*it).second->getTime())
					continue;
				else
					total += (*it).second->getDeliveryFee();
			}
		}
		else if ((*it).second->getDate() < date1) // se a data for antes da data de início
			continue;
		else // se a data e a data de início forem iguais->testar horas
		{
			if (date2 < (*it).second->getDate()) // se a data do fim for antes da data
				continue;
			else
			{
				if ((*it).second->getTime() < hour1) // se a hora da data for antes da hora da data de inicio
					continue;
				else
					total += (*it).second->getDeliveryFee();
			}
		}
	}
	cout << "Profit made in the defined period: " << total;

	cout << "\n>> ";
	cin.ignore();
	*/
}




void Base::changeBase() {
	list<string> options = { "address" };
	
	bool invalidOption;
	string strChoice;
	unsigned index = 0;

	cout << "Pick the field you want to change information of:" << endl;
	list<string>::iterator optionsIt;
	int attributeChoice;
	do {
		index = 0;
		invalidOption = false;

		for (optionsIt = options.begin(); optionsIt != options.end(); ++optionsIt, ++index) {
			cout << index + 1 << ". " << (*optionsIt) << endl;
		}

		try {
			cout << ">> ";
			getline(cin, strChoice);

			if (cin.eof()) {
				cin.clear();
				return ;
			}

			attributeChoice = stoi(strChoice);

			if (attributeChoice < 1 || attributeChoice > options.size()) {
				invalidOption = true;
			}
		}
		catch (...) {
			invalidOption = true;
		}

		cout << endl;

	} while (invalidOption);

	
	Address newAddress;
	bool invalidAddress = false;
	string fullAddress;
	vector<string> areaOfInf = this->getAreaOfInfluence();

	switch (attributeChoice) {
		// Address
		case 1:
			do {
				invalidAddress = false;

				cout << "Current Address: " << this->getAddress() << endl;
				cout << "Updated Address (District / Town / Street / No / Floor / Latitude / Longitude): " << endl;
				getline(cin, fullAddress);

				if (cin.eof()) {
					cin.clear();
					return;
				}

				try { //esta exceção não está testada
					bool teste = address.parse(fullAddress);
					if (!teste)
						throw InvalidAddressException(fullAddress);
					// if it doesnt belong to the are of influence it is considered invalid
					if (find(areaOfInf.begin(), areaOfInf.end(), address.get_town()) == areaOfInf.end()) {
						throw InvalidCityException(address.get_town());
					}
				}

				catch (InvalidAddressException & a) {
					invalidAddress = true;
					cout << a;
					cout << "Try Again!" << endl << endl;
				}
				catch (InvalidCityException & c)
				{
					invalidAddress = true;
					cout << c;
					cout << "Try Again!" << endl << endl;
				}

				cout << endl;
			} while (invalidAddress);
			this->setAddress(address);
			
			break;

	}

	cout << "Based successfully edited! (Enter to continue)" << endl;
	cout << ">> ";
	cin.ignore();
}

bool Base::addClient() { //usar em try para apanhar execao blacklisted

	try
	{
		Client c;

		// HARD CODED FOR BASE PORTO
		// vector<string> areaOfInfluence = { "Porto", "Matosinhos", "Vila Nova de Gaia", "Gondomar", "Maia"};
		string baseName = this->getDistrict();
		c.set_base(this);


	



		// name input
		bool invalidName;
		string name;
		do {
			try
			{
				invalidName = false;

				cout << "Name: ";
				getline(cin, name);
				if (!isString(name))
					throw InvalidStringException(name);
			}
			catch (InvalidStringException & s)
			{
				invalidName = true;
				cout << s;
				cout << "Try Again!" << endl << endl;

			}
	

			/*	if (cin.eof()) {
					cin.clear();
					return;
				}*/

		} while (invalidName);

		c.set_name(name);

		if (find(Base::blacklist.begin(), Base::blacklist.end(), c.get_name()) != Base::blacklist.end()) { //EXCEÇÂO BLACKLISTED
			throw BlackListedException(c.get_name());



		}





		// nif input
		bool invalidNif;
		string strNif;
		int nif;
		do {
			invalidNif = false;

			cout << "NIF: ";
			getline(cin, strNif);

			//if (cin.eof()) {
			//	cin.clear();
			//	return;
			//}

			try {
				if (!isNumber(strNif) || strNif.size() != 9)
					throw InvalidNIFException(strNif);
			}

			catch (InvalidNIFException & n) {
				invalidNif = true;
				cout << n;
				cout << "Try Again!" << endl << endl;
			}

		} while (invalidNif);

		c.set_NIF(stoi(strNif));

		// address input
		bool invalidAddress;
		Address address;
		vector<string> areaOfInf = this->getAreaOfInfluence();

		do {
			invalidAddress = false;

			string fullAddress;
			cout << "Address (District / Town / Street / No / Floor / Latitude / Longitude):" << endl;
			getline(cin, fullAddress);

			//if (cin.eof()) {
			//	cin.clear();
			//	return;
			//}

			try {
				bool teste = address.parse(fullAddress);
				if (!teste)
					throw InvalidAddressException(fullAddress);
				// if it doesnt belong to the area of influence it is considered invalid
				if (find(areaOfInf.begin(), areaOfInf.end(), address.get_town()) == areaOfInf.end()) {
					throw InvalidCityException(address.get_town());
				}
			}

			catch (InvalidAddressException & a) {
				invalidAddress = true;
				cout << a;
				cout << "Try Again!" << endl << endl;

			}
			catch (InvalidCityException & c)
			{
				cout << c;
				cout << ">>";
				cin.ignore();
				return false;
			}

		} while (invalidAddress);

		c.set_address(address);

		// orders vector that starts empty
		map<int, Order*> clientOrders = {};

		c.set_orders(clientOrders);


		Client * ptr = new Client;
		*ptr = c;

		clients.push_back(ptr);

		cout << endl;
		cout << "Client added successfully" << endl;
		cout << ">> ";
		cin.ignore();
	}
	catch (BlackListedException & b)
	{
		cout << b;
		cout << ">> ";
		cin.ignore();
		return false;

	}
	return true;
}

void Base::changeClient() {

	cout << "Pick the client you want to change information about:" << endl;

	vector<Client*>::iterator it;
	bool invalidOption;
	string strChoice;
	int clientChoice;
	unsigned index = 0;
	
	
	do {
		index = 0;
		invalidOption = false;

		for (it = clients.begin(); it != clients.end(); ++it, ++index) {
			cout << index + 1 << ". " << (*it)->get_name() << endl;
		}
		try {
			cout << "0. Go Back" << endl;
			cout << ">> ";
			getline(cin, strChoice);
			if (strChoice == "0")
			{
				cin.clear();
				utils::clear_screen();
				return;
			}
			//if (cin.eof()) {
			//	cin.clear();
			//	return;
			//}
			
			if (!isNumber(strChoice))
				throw InvalidNumberException(strChoice);
			clientChoice = stoi(strChoice);

			if (InvalidOptions(clients.size(), clientChoice)) {
				throw InvalidOptionException(clientChoice);
			}
		}
		catch (InvalidOptionException & o) {
			invalidOption = true;
			cout << o;
			cout << "Try Again!" << endl << endl;
		}
		catch (InvalidNumberException & s)
		{
			invalidOption = true;
			cout << s;
			cout << "Try Again!" << endl << endl;
		}
		
		cout << endl;

	} while (invalidOption);
	
	clientChoice--; // not to excede the max index available
	
	clients.at(clientChoice)->edit(this);
}

void Base::removeClient() {

	cout << "Pick the client you want to remove:" << endl;

	vector<Client*>::iterator it;
	bool invalidOption;
	string strChoice;
	int clientChoice;
	unsigned index = 0;


	do {
		index = 0;
		invalidOption = false;

		for (it = clients.begin(); it != clients.end(); ++it, ++index) {
			cout << index + 1 << ". " << (*it)->get_name() << endl;
		}
		try {
			cout << "0. Go Back" << endl;
			cout << ">> ";
			getline(cin, strChoice);

			if (strChoice == "0") {
				cin.clear();
				utils::clear_screen();
				return;
			}

			if (!isNumber(strChoice))
				throw InvalidNumberException(strChoice);

			clientChoice = stoi(strChoice);

			if (InvalidOptions(clients.size(), clientChoice)) {
				throw InvalidOptionException(clientChoice);
			}
		}
		catch (InvalidOptionException & o) {
			invalidOption = true;
			cout << o;
			cout << "Try Again!" << endl << endl;
		}
		catch (InvalidNumberException & s)
		{
			invalidOption = true;
			cout << s;
			cout << "Try Again!" << endl << endl;
		}

		cout << endl;

	} while (invalidOption);

	clientChoice--; // not to excede the max index available
	
	//loop a apagar todas as orders dele !!

	clients.erase(clients.begin() + clientChoice);

	cout << "Client removed successfully" << endl;
	cout << ">> ";
	cin.ignore();

}

void Base::addWorker(){ //Continuar a adicionar exceçoes
	
	// checks if there is a manager (if it exists, it is the first element of the vector workers
	bool managerExists = (dynamic_cast<Admin*>(workers.at(0))->get_role() == "manager") ? true : false;
	

	// type of worker input
	bool invalidWorkerType;
	string strWorkerType;
	int workerType;
	do {
		try
		{
			invalidWorkerType = false;
			cout << "Adding an Administrator or a Delivery person?" << endl;
			cout << "1. Administrator" << endl;
			cout << "2. Delivery" << endl;
			cout << "0. Go Back" << endl;
			cout << ">> ";
			getline(cin, strWorkerType);
			if (strWorkerType == "0")
			{
				cin.clear();
				utils::clear_screen();
				return;
			}
			if (!isNumber(strWorkerType))
				throw InvalidNumberException(strWorkerType);
			if (InvalidOptions(2, stoi(strWorkerType)))
				throw InvalidOptionException(stoi(strWorkerType));
		}
		catch(InvalidOptionException & o)
		{
			invalidWorkerType = true;
			cout << o;
			cout << "Try Again!" << endl << endl;
		}
		catch (InvalidNumberException & s)
		{
			invalidWorkerType = true;
			cout << s;
			cout << "Try Again!" << endl << endl;
		}
	} while (invalidWorkerType);

	workerType = stoi(strWorkerType);
	
	cout << endl;
	// name input
	bool invalidName;
	string name;
	do {
		try
		{
			invalidName = false;
			cout << "Name: ";
			getline(cin, name);
			if (!isString(name))
				throw InvalidStringException(name);
			cout << endl;
		}
		catch (InvalidStringException & s)
		{
			invalidName = true;
			cout << s;
			cout << "Try Again" << endl << endl;
		}

		//if (cin.eof()) {
		//	cin.clear();
		//	return;
		//}

		
	} while (invalidName);

	
	
	// nif input
	bool invalidNif;
	string strNif;
	int nif;
	do {
		invalidNif = false;

		cout << "NIF: ";
		getline(cin, strNif);

		//if (cin.eof()) {
		//	cin.clear();
		//	return;
		//}

		try {
			if (!isNumber(strNif) || strNif.size() != 9)
				throw InvalidNIFException(strNif);
		}

		catch (InvalidNIFException & n) {
			invalidNif = true;
			cout << n;
			cout << "Try Again!" << endl << endl;
		}

		cout << endl;
	} while (invalidNif);

	

	// birthday input
	bool invalidBirthday;
	Date_time birthday;
	bool teste;
	do {
		invalidBirthday = false;

		string fullBirthday;
		cout << "Birthday: ";
		getline(cin, fullBirthday);

		//if (cin.eof()) {
		//	cin.clear();
		//	return;
		//}

		try {
			teste = birthday.parse(fullBirthday);
			if (!teste || !isDateValid(birthday))
			{
				throw InvalidDateException(fullBirthday);
			}
		}
		catch (InvalidDateException & d) {
			invalidBirthday = true;
			cout << d;
			cout << "Try Again!" << endl << endl;
		}

		cout << endl;
	} while (invalidBirthday);

	
	Admin admin;
	Delivery delivery;
	Vehicle vehicle;

	// up until this point, all workers are the same. from now on, they'll follow different "paths"
	bool invalidManagerInput = false;
	string strManagerInput;
	int managerInput;

	bool invalidRoleInput = false;
	string roleInput;
	
	string vehicleBrand;
	string vehicleType;

	bool invalidRegistrationDate = false;
	string strRegistrationDate;
	Date_time registrationDate; 


	// chose Admin
	if (workerType == 1) {
		
		if (!managerExists) { // if there isn't a manager yet gives the option that the admin to be the manager

			do {
				invalidManagerInput = false;

				cout << "Is this Administrator the Manager? " << endl;
				cout << "1. Yes" << endl;
				cout << "2. No" << endl;
				cout << ">> ";
				getline(cin, strManagerInput);

				if (cin.eof()) {
					cin.clear();
					return;
				}

				try {
					if (!isNumber(strManagerInput))
						throw InvalidNumberException(strManagerInput);
					managerInput = stoi(strManagerInput);
					if (InvalidOptions(2, managerExists))
						throw InvalidOptionException(managerExists);
				}
				catch (InvalidOptionException & o) {
					invalidManagerInput = true;
					cout << o;
					cout << "Try Again!" << endl << endl;
				}
				catch (InvalidNumberException & s)
				{
					invalidManagerInput = true;
					cout << s;
					cout << "Try Again!" << endl << endl;
				}

				cout << endl;
			} while (invalidManagerInput);

		
			if (managerInput == 1) { // if it chooses to be manager
				admin.set_role("manager");
			}

			
			else { // if it chooses not to be manager
				do {
					try
					{
						invalidRoleInput = false;
						cout << "Role: ";
						getline(cin, roleInput);
						if (!isString(roleInput))
							throw InvalidStringException(roleInput);

						//if (cin.eof()) {
						//	cin.clear();
						//	return;
						//}

						// if it chose not to be manager it can't then be manager
						if (roleInput == "manager") {
							invalidRoleInput = true;
						}

						cout << endl;
					}
					catch (InvalidStringException & s)
					{
						invalidRoleInput = true;
						cout << s;
						cout << "Try Again!" << endl << endl;
					}
					
				} while (invalidRoleInput);

				admin.set_role(roleInput);
			}

		}
		
		else { // if there is already a manager

			do {
				try
				{
					invalidRoleInput = false;

					cout << "Role: ";
					getline(cin, roleInput);
					if (!isString(roleInput))
						throw InvalidStringException(roleInput);
					//if (cin.eof()) {
					//	cin.clear();
					//	return;
					//}

					// can't be manager
					if (roleInput == "manager") {
						invalidRoleInput = true;
					}

					cout << endl;
				}
				catch (InvalidStringException & s)
				{
					invalidRoleInput = true;
					cout << s;
					cout << "Try Again!" << endl << endl;

				}
			} while (invalidRoleInput);

			admin.set_role(roleInput);
		}

		admin.set_name(name);
		admin.set_NIF(stoi(strNif));
		admin.set_birthday(birthday);

		Admin * ptr3 = new Admin;
		*ptr3 = admin;

		if (ptr3->get_role() == "manager") {
			workers.insert(workers.begin(), ptr3);
		}
		else {
			workers.insert(workers.begin() + 1, ptr3);
		}
	}

	// chose Delivery
	else if (workerType == 2) {
		bool retry = true;
		do
		{
			try
			{
				retry = false;
				cout << "Vehicle Brand: ";
				getline(cin, vehicleBrand);
				if (!isString(vehicleBrand))
					throw InvalidStringException(vehicleBrand);

			}
			catch (InvalidStringException & s)
			{
				retry = true;
				cout << s;
				cout << "Try Again!" << endl << endl;
			}


			//if (cin.eof()) {
			//	cin.clear();
			//	return;
			//}


		} while (retry);
	
		cout << endl;
		retry = true;
		do {
			try {
				retry = false;
				cout << "Vehicle Type: ";
				getline(cin, vehicleType);
				if (!isString(vehicleType))
					throw InvalidStringException(vehicleType);
			}
			catch (InvalidStringException & s)
			{
				retry = true;
				cout << s;
				cout << "Try Again!" << endl << endl;

			}

		} while (retry);
	
		//if (cin.eof()) {
		//	cin.clear();
		//	return;
		//}

		cout << endl;
		do {
			invalidRegistrationDate = false;
			cout << "Registration Date: ";
			getline(cin, strRegistrationDate);

			//if (cin.eof()) {
			//	cin.clear();
			//	return;
			//}

			try {
				teste = registrationDate.parse(strRegistrationDate);
				if (!teste || !isDateValid(registrationDate))
					throw InvalidDateException(strRegistrationDate);
			}

			catch (InvalidDateException & d) {
				invalidRegistrationDate = true;
				cout << d;
				cout << "Try Again!" << endl << endl;
			}

			cout << endl;
		} while (invalidRegistrationDate);

		vehicle.set_brand(vehicleBrand);
		vehicle.set_type(vehicleType);
		vehicle.set_registrationDate(registrationDate);

		// Vehicle * ptr2 = new Vehicle;
		// *ptr2 = vehicle;
		delivery.set_vehicle(vehicle);

		delivery.set_name(name);
		delivery.set_NIF(stoi(strNif));
		delivery.set_birthday(birthday);

		Delivery * ptr4 = new Delivery;
		*ptr4 = delivery;
		workers.push_back(ptr4);


	}
	
	cout << endl;
	cout << "Worker successfully added! (Enter to continue)" << endl;
	cout << ">> ";
	cin.ignore();
}

void Base::changeWorker() {

	Admin *adminCheck;
	Delivery *deliveryCheck;
	int firstDeliveryIndex = 0;
	bool foundFirstDelivery = false;
	bool invalidOption = false;
	string strWorkerChoice;
	int workerChoice;


	cout << "Pick the worker you want to change information about:" << endl;

	do {
		invalidOption = false;
		foundFirstDelivery = false;

		for (size_t i = 0; i < workers.size(); i++) {
			adminCheck = dynamic_cast<Admin*> (workers.at(i));
			if (adminCheck != NULL) {
				firstDeliveryIndex++;
				// if i == 0, then i converts to false, so !i == true
				// 0 converts to false, any other convertable to bool convert to true
				if (!i) {
					cout << "Administrators" << endl;
					cout << i + 1 << ". " << adminCheck->get_name() << " (" << adminCheck->get_role() << ")" << endl;
				}
				else {
					cout << i + 1 << ". " << adminCheck->get_name() << endl;
				}

			}
			else {
				deliveryCheck = dynamic_cast<Delivery*> (workers.at(i));
				if (!foundFirstDelivery) {
					foundFirstDelivery = true;
					cout << "\nDeliverers" << endl;
				}
				cout << i + 1 << ". " << deliveryCheck->get_name() << endl;
			}
		}

		// cout << "\nFirst Index Delivery: " << firstDeliveryIndex << endl;

		try {
			cout << endl << "0. Go Back" << endl;
			cout << ">> ";
			getline(cin, strWorkerChoice);
			if (strWorkerChoice == "0")
			{
				cin.clear();
				utils::clear_screen();
				return;
			}

			if (!isNumber(strWorkerChoice))
				throw InvalidNumberException(strWorkerChoice);
			workerChoice = stoi(strWorkerChoice);

			if (InvalidOptions(workers.size(), workerChoice)) {
				throw InvalidOptionException(workerChoice);
			}
		}
		catch (InvalidOptionException & o) {
			invalidOption = true;
			cout << o;
			cout << "Try Again!" << endl << endl;
		}
		catch (InvalidNumberException & s)
		{
			invalidOption = true;
			cout << s;
			cout << "Try Again!" << endl << endl;
		}
		cout << endl;

		// cout << dynamic_cast<Delivery*>(workers.at(3))->get_vehicle().get_brand() << endl;

	} while (invalidOption);

	workerChoice--; // not to excede the max index available



	utils::clear_screen();



	// cout << workers.at(workerChoice)->get_name() << endl;
	unsigned index = 0;
	string strAdminAttributeChoice;
	int adminAttributeChoice;

	string strDelivAttributeChoice;
	int delivAttributeChoice;

	list<string>::iterator optionsIt;
	list<string> adminOptions = { "Name", "Nif", "Birthday", "Wage", "Role" };
	list<string> deliveryOptions = { "Name", "Nif", "Birthday", "Wage", "Vehicle" };
	cout << "Pick the field you want to change information of:" << endl;
	// cout << dynamic_cast<Delivery*>(workers.at(workerChoice))->get_vehicle().get_brand() << endl;
	// auto x = dynamic_cast<Delivery*>(workers.at(workerChoice));
	// Admin newAdmin;
	// Delivery newDelivery;

	auto adminObject = dynamic_cast<Admin*>(workers.at(workerChoice));
	auto delivObject = dynamic_cast<Delivery*>(workers.at(workerChoice));

	string newName;

	string strNewNif;
	int newNif = 0;
	bool invalidNif = false;

	bool invalidBirthday = false;
	Date_time newBirthday;
	string fullBirthday;

	bool invalidWage = false;
	string strNewWage;
	int newWage;

	bool adminExists = false;
	bool invalidRole = false;
	string newRole;

	bool invalidRegistrationDate = false;
	string newVehicleBrand;
	string newVehicleType;
	string strNewRegistDate;
	Date_time newRegistDate;
	Vehicle newVehicle;
	bool retry = true;
	bool teste;
	// worker chosen is an Admin
	if (workerChoice < firstDeliveryIndex) {
		do {
			index = 0;
			invalidOption = false;

			for (optionsIt = adminOptions.begin(); optionsIt != adminOptions.end(); ++optionsIt, ++index) {
				cout << index + 1 << ". " << (*optionsIt) << endl;
			}

			try {
				cout << ">> ";
				getline(cin, strAdminAttributeChoice);
				//if (cin.eof())
				//{
				//	cin.clear();
				//	return;
				//}

				if (!isNumber(strAdminAttributeChoice))
					throw InvalidNumberException(strAdminAttributeChoice);
				adminAttributeChoice = stoi(strAdminAttributeChoice);

				if (InvalidOptions(adminOptions.size(), adminAttributeChoice)) {
					throw InvalidOptionException(adminAttributeChoice);
				}
			}

			catch (InvalidOptionException & o) {
				invalidOption = true;
				cout << o;
				cout << "Try Again!" << endl;
			}
			catch (InvalidNumberException & s)
			{
				invalidOption = true;
				cout << s;
				cout << "Try Again!" << endl << endl;
			}

			cout << endl;
		} while (invalidOption);


		switch (adminAttributeChoice) {

			// Name
			case 1:
				do
				{
					retry = false;
					try
					{
						cout << "Current Name: " << adminObject->get_name() << endl;
						cout << "Updated Name: ";
						getline(cin, newName);
						if (!isString(newName))
							throw InvalidStringException(newName);
					}
					catch (InvalidStringException & s)
					{
						retry = true;
						cout << s;
						cout << "Try Again!" << endl << endl;
					}
				} while (retry);


				//if (cin.eof()) {
				//	cin.clear();
				//	return;
				//}

				cout << endl;
				adminObject->set_name(newName);
				break;

			// Nif
			case 2:
				do {
					invalidNif = false;

					cout << "Current Nif: " << adminObject->get_NIF() << endl;
					cout << "Updated Nif: ";
					getline(cin, strNewNif);

					//if (cin.eof()) {
					//	cin.clear();
					//	return;
					//}

					try {
						if (!isNumber(strNewNif) || strNewNif.size() != 9)
							throw InvalidNIFException(strNewNif);
					}

					catch (InvalidNIFException & n) {
						invalidNif = true;
						cout << n;
						cout <<"Try Again!" << endl << endl;
					}
					

					cout << endl;
				} while (invalidNif);
				adminObject->set_NIF(newNif);
				break;

			// Birthday
			case 3:
				do {
					invalidBirthday = false;

					cout << "Current Birthday: " << adminObject->get_birthday() << endl;
					cout << "Updated Birthday: ";
					getline(cin, fullBirthday);

					//if (cin.eof()) {
					//	cin.clear();
					//	return;
					//}

					try {
						teste = newBirthday.parse(fullBirthday);
						if (!teste || !isDateValid(newBirthday))
							throw InvalidDateException(fullBirthday);
					}
					catch (InvalidDateException & d) {
						invalidBirthday = true;
						cout << d;
						cout << "Try Again!" << endl << endl;
					}

					cout << endl;

					cout << endl;
				} while (invalidBirthday);
				adminObject->set_birthday(newBirthday);
				break;

			// Wage
			case 4:
				do {
					invalidWage = false;

					cout << "Current Wage: " << adminObject->get_wage() << endl;
					cout << "Updated Wage: ";
					getline(cin, strNewWage);

					//if (cin.eof()) {
					//	cin.clear();
					//	return;
					//}

					try {
						if (!isNumber(strNewWage))
							throw InvalidNumberException(strNewWage);
					}
					catch (InvalidNumberException & n) {
						invalidWage = true;
						cout << n;
						cout << "Try Again!" << endl << endl;
					}

					cout << endl;
				} while (invalidWage);
				adminObject->set_wage(stoi(strNewWage));
				break;

			// Role
			case 5:
				adminExists = ((dynamic_cast<Admin*>(workers.at(0))->get_role() == "manager") && (workerChoice != 0)) ? true : false;
				do {
					try
					{
						invalidRole = false;
						cout << "Current role: " << adminObject->get_role() << endl;
						cout << "Updated role: ";
						getline(cin, newRole);
						if (!isString(newRole))
							throw InvalidStringException(newRole);

						//if (cin.eof()) {
						//	cin.clear();
						//	return;
						//}

						if (newRole == "manager" && adminExists) {
							invalidRole = true;
						}

						cout << endl;
					}
					catch (InvalidStringException & s)
					{
						invalidRole = true;
						cout << s;
						cout << "Try Again!" << endl << endl;
					}
				} while (invalidRole);
				adminObject->set_role(newRole);
				break;
		}
	}

	// worker chosen is an Delivery
	else if (workerChoice >= firstDeliveryIndex) {

		do {
			index = 0;
			invalidOption = false;

			for (optionsIt = deliveryOptions.begin(); optionsIt != deliveryOptions.end(); ++optionsIt, ++index) {
				cout << index + 1 << ". " << (*optionsIt) << endl;
			}

			try {
				cout << ">> ";
				getline(cin, strDelivAttributeChoice);
				if (cin.eof())
				{
					cin.clear();
					return;
				}

				if (!isNumber(strDelivAttributeChoice))
					throw InvalidNumberException(strDelivAttributeChoice);
				delivAttributeChoice = stoi(strDelivAttributeChoice);

				if (InvalidOptions(deliveryOptions.size(), delivAttributeChoice)) {
					throw InvalidOptionException(delivAttributeChoice);
				}
			}

			catch (InvalidOptionException & o) {
				invalidOption = true;
				cout << o;
				cout << "Try Again!" << endl << endl;
			}
			catch (InvalidNumberException & s)
			{
				invalidOption = true;
				cout << s;
				cout << "Try Again!" << endl << endl;
			}

			cout << endl;
		} while (invalidOption);



		switch (delivAttributeChoice) {

			// Name
			case 1:
				do
				{
					try {
						retry = false;
						cout << "Current Name: " << delivObject->get_name() << endl;
						cout << "Updated Name: ";
						getline(cin, newName);
						if (!isString(newName))
							throw InvalidStringException(newName);
					}
					catch (InvalidStringException & s)
					{
						retry = true;
						cout << s;
						cout << "Try Again!" << endl << endl;
					}
		

				} while (retry);

				//if (cin.eof()) {
				//	cin.clear();
				//	return;
				//}

				cout << endl;
				delivObject->set_name(newName);
				break;

			// Nif
			case 2:
				do {
					invalidNif = false;

					cout << "Current Nif: " << delivObject->get_NIF() << endl;
					cout << "Updated Nif: ";
					getline(cin, strNewNif);

					//if (cin.eof()) {
					//	cin.clear();
					//	return;
					//}

					try {
						if (!isNumber(strNewNif) || strNewNif.size() != 9)
							throw InvalidNIFException(strNewNif);
					}

					catch (InvalidNIFException & n) {
						invalidNif = true;
						cout << n;
						cout << "Try Again!" << endl << endl;
					}
					

					cout << endl;
				} while (invalidNif);
				delivObject->set_NIF(newNif);
				break;

			// Birthday
			case 3:
				do {
					invalidBirthday = false;

					cout << "Current Birthday: " << delivObject->get_birthday() << endl;
					cout << "Updated Birthday: ";
					getline(cin, fullBirthday);

					//if (cin.eof()) {
					//	cin.clear();
					//	return;
					//}
					
					try {
						teste = newBirthday.parse(fullBirthday);
						if (!teste || !isDateValid(newBirthday))
							throw InvalidDateException(fullBirthday);
					}
					catch (InvalidDateException & d) {
						invalidBirthday = true;
						cout << d;
						cout << "Try Again!" << endl << endl;
					}

					cout << endl;

					cout << endl;
				} while (invalidBirthday);
				delivObject->set_birthday(newBirthday);
				break;

			// Wage
			case 4:
				do {
					invalidWage = false;

					cout << "Current Wage: " << delivObject->get_wage() << endl;
					cout << "Updated Wage: ";
					getline(cin, strNewWage);

					//if (cin.eof()) {
					//	cin.clear();
					//	return;
					//}

					try {
						if (!isNumber(strNewWage))
							throw InvalidNumberException(strNewWage);
					}
					catch (InvalidNumberException & n) {
						invalidWage = true;
						cout << n;
						cout << "Try Again!" << endl << endl;
					}

					cout << endl;
				} while (invalidWage);
				delivObject->set_wage(stoi(strNewWage));
				break;

			// Vehicle
			case 5:
				retry = true;
				do
				{
					try
					{
						retry = false;
						cout << "Current Brand: " << delivObject->get_vehicle().get_brand() << endl;
						cout << "Updated Brand: ";
						getline(cin, newVehicleBrand);
						if (!isString(newVehicleBrand))
							throw InvalidStringException(newVehicleBrand);

					}
					catch (InvalidStringException & s)
					{
						retry = true;
						cout << s;
						cout << "Try Again!" << endl << endl;
					}
				} while (retry);
	
				//if (cin.eof()) {
				//	cin.clear();
				//	return;
				//}

				newVehicle.set_brand(newVehicleBrand);
				cout << endl;

				
				retry = true;
				do
				{
					try
					{
						retry = false;
						cout << "Current Type: " << delivObject->get_vehicle().get_type() << endl;
						cout << "Updated Type: ";
						getline(cin, newVehicleType);
						if (!isString(newVehicleType))
							throw InvalidStringException(newVehicleType);
					}
					catch (InvalidStringException & s)
					{
						retry = true;
						cout << s;
						cout << "Try Again!" << endl << endl;
					}

				} while (retry);

				//if (cin.eof()) {
				//	cin.clear();
				//	return;
				//}

				newVehicle.set_type(newVehicleType);
				cout << endl;

				do {
					invalidRegistrationDate = false;

					cout << "Current Registration Date: " << delivObject->get_vehicle().get_registration_date() << endl;
					cout << "Updated Registration Date: ";
					getline(cin, strNewRegistDate);

	/*				if (cin.eof()) {
						cin.clear();
						return;
					}*/

					try {
						teste = newRegistDate.parse(strNewRegistDate);
						if (!teste || !isDateValid(newRegistDate))
							throw InvalidDateException(strNewRegistDate);
					}

					catch (InvalidDateException & d) {
						invalidRegistrationDate = true;
						cout << d;
						cout << "Try Again!" << endl << endl;
					}
					cout << endl;
				} while (invalidRegistrationDate);
				newVehicle.set_registrationDate(newRegistDate);

				// Vehicle * ptrNewVehicle = new Vehicle;
				// *ptrNewVehicle = newVehicle;
				delivObject->set_vehicle(newVehicle);
				break;
		
		}
	}

	cout << endl;
	cout << "Worker successfully edited! (Enter to continue)" << endl;
	cout << ">> ";
	cin.ignore();
}

void Base::removeWorker() {

	Admin *adminCheck;
	Delivery *deliveryCheck;
	int firstDeliveryIndex = 0;
	bool foundFirstDelivery = false;
	bool invalidOption = false;
	string strWorkerChoice;
	int workerChoice;


	cout << "Pick the worker you want to remove:" << endl;

	do {
		invalidOption = false;
		foundFirstDelivery = false;

		for (size_t i = 0; i < workers.size(); i++) {
			adminCheck = dynamic_cast<Admin*> (workers.at(i));
			if (adminCheck != NULL) {
				firstDeliveryIndex++;
				// if i == 0, then i converts to false, so !i == true
				// 0 converts to false, any other convertable to bool convert to true
				if (!i) {
					cout << "Administrators" << endl;
					cout << i + 1 << ". " << adminCheck->get_name() << " (" << adminCheck->get_role() << ")" << endl;
				}
				else {
					cout << i + 1 << ". " << adminCheck->get_name() << endl;
				}

			}
			else {
				deliveryCheck = dynamic_cast<Delivery*> (workers.at(i));
				if (!foundFirstDelivery) {
					foundFirstDelivery = true;
					cout << "\nDeliverers" << endl;
				}
				cout << i + 1 << ". " << deliveryCheck->get_name() << endl;
			}
		}

		// cout << "\nFirst Index Delivery: " << firstDeliveryIndex << endl;

		try {
			cout << endl << "0. Go Back" << endl;
			cout << ">> ";
			getline(cin, strWorkerChoice);
			if (strWorkerChoice == "0")
			{
				cin.clear();
				utils::clear_screen();
				return;
			}

			if (!isNumber(strWorkerChoice))
			{
				throw InvalidNumberException(strWorkerChoice);
			}
			workerChoice = stoi(strWorkerChoice);

			if (InvalidOptions(workers.size(), workerChoice)) {
				throw InvalidOptionException(workerChoice);
			}
		}
		catch (InvalidOptionException & o) {
			invalidOption = true;
			cout << o;
			cout << "Try Again!" << endl << endl;
		}
		catch (InvalidNumberException & s)
		{
			invalidOption = true;
			cout << s;
			cout << "Try Again!" << endl << endl;
		}
		cout << endl;

		// cout << dynamic_cast<Delivery*>(workers.at(3))->get_vehicle().get_brand() << endl;

	} while (invalidOption);

	workerChoice--; // not to excede the max index available

	workers.erase(workers.begin() + workerChoice);

	cout << "Worker removed successfully" << endl;
	cout << ">> ";
	cin.ignore();

}

void Base::addRestaurant() {

	Restaurant r;

	// HARD CODED FOR BASE PORTO
	// vector<string> areaOfInfluence = { "Porto", "Matosinhos", "Vila Nova de Gaia", "Gondomar", "Maia" };
	string baseName = this->getDistrict();
	r.setBase(this);

	// name input
	bool invalidName = true;
	string name;
	do {
		try
		{
			invalidName = false;

			cout << "Name: ";
			getline(cin, name);
			if (!isString(name))
				throw InvalidStringException(name);
		}
		catch (InvalidStringException & s)
		{
			invalidName = true;
			cout << s;
			cout << "Try Again!" << endl << endl;
		}

		//if (cin.eof()) {
		//	cin.clear();
		//	return;
		//}

	} while (invalidName);

	r.setName(name);

	// address input
	bool invalidAddress;
	Address address;
	string fullAddress;
	vector<string> areaOfInf = this->getAreaOfInfluence();

	do {
		invalidAddress = false;
	
		cout << "Address: ";
		getline(cin, fullAddress);

		//if (cin.eof()) {
		//	cin.clear();
		//	return;
		//}

		try {
			bool teste = address.parse(fullAddress);
			if (!teste)
				throw InvalidAddressException(fullAddress);

			// if it doesnt belong to the are of influence it is considered invalid
			if (find(areaOfInf.begin(), areaOfInf.end(), address.get_town()) == areaOfInf.end()) {
				throw InvalidCityException(address.get_town());
			}
		}

		catch (InvalidAddressException & a) {
			invalidAddress = true;
			cout << a;
			cout << "Try Again!" << endl << endl;
		}
		catch (InvalidCityException & c)
		{
			cout << c;
			cin.ignore();
			//utils::clear_screen();
			cout << ">>";
			return;
		}

	} while (invalidAddress);

	r.setAddress(address);

	// products input
	bool invalidProduct;
	string strProduct;
	Product product;
	vector<Product*> productsVec;

	bool notEnded = true;

	cout << "List of Products (Name : Type : Price), type 'done' to stop input" << endl;

	do {
		
		do {
			invalidProduct = false; //Implementar uma exceção invalid product

			getline(cin, strProduct);

			//if (cin.eof()) {
			//	cin.clear();
			//	return;
			//}

			if (strProduct == "done") {
				notEnded = false;
				break;
			}

			try {
				bool teste = product.parse(strProduct);
				if (!teste)
					throw InvalidProductException(strProduct);
				Product * productPtr = new Product;
				*productPtr = product;
				productsVec.push_back(productPtr);
			}
			catch (InvalidProductException & p) {
				invalidProduct = true;
				cout << p;
				cout << "Try Again!" << endl << endl;
			}
		} while (invalidProduct);

	} while (notEnded);

	r.setProducts(productsVec);


	// cuisine types
	set<string> cuisineTypes;
	for (auto & prod : r.get_products()) {
		cuisineTypes.insert(prod->get_cuisine_type());
	}

	
	r.setCuisineTypes(cuisineTypes);
	

	
	// price average calculated form the vector of products
	r.setPriceAverage();

	Restaurant * restaurantPtr = new Restaurant;
	*restaurantPtr = r;
	restaurants.push_back(restaurantPtr);

	cout << endl;
	cout << "Restaurant successfully added! (Enter to continue)" << endl;
	cout << ">> ";
	cin.ignore();
}

void Base::changeRestaurant() {

	vector<Restaurant*>::iterator it;
	bool invalidOption;
	string strChoice;
	int restaurantChoice;
	unsigned index = 0;

	cout << "Pick the restaurant you want to change information about:" << endl;

	do {
		index = 0;
		invalidOption = false;

		for (it = restaurants.begin(); it != restaurants.end(); ++it, ++index) {
			cout << index + 1 << ". " << (*it)->get_name() << endl;
		}
		try {
			cout << "0. Go Back" << endl;
			cout << ">> ";
			getline(cin, strChoice);
			if (strChoice == "0")
			{
				cin.clear();
				utils::clear_screen();
				return;
			}

			if (!isNumber(strChoice))
				throw InvalidNumberException(strChoice);
			restaurantChoice = stoi(strChoice);

			if (InvalidOptions(restaurants.size(), restaurantChoice)) {
				throw InvalidOptionException(restaurantChoice);
			}
		}
		catch (InvalidOptionException & o) {
			invalidOption = true;
			cout << o;
			cout << "Try Again!" << endl << endl;
		}
		catch (InvalidNumberException & s)
		{
			invalidOption = true;
			cout << s;
			cout << "Try Again!" << endl << endl;
		}
		cout << endl;

	} while (invalidOption);

	restaurantChoice--;	// not to excede the max index available

	list<string> options = { "name", "address", "products" };
	cout << "Pick the field you want to change information of:" << endl;

	list<string>::iterator optionsIt;
	int attributeChoice;
	do {
		index = 0;
		invalidOption = false;

		for (optionsIt = options.begin(); optionsIt != options.end(); ++optionsIt, ++index) {
			cout << index + 1 << ". " << (*optionsIt) << endl;
		}

		try {
			cout << ">> ";
			getline(cin, strChoice);

			//if (cin.eof())
			//{
			//	cin.clear();
			//	return;
			//}

			if (!isNumber(strChoice))
				throw InvalidNumberException(strChoice);
			attributeChoice = stoi(strChoice);

			if (InvalidOptions(options.size(), attributeChoice)) {
				throw InvalidOptionException(attributeChoice);
			}
		}
		catch (InvalidOptionException & o) {
			invalidOption = true;
			cout << o;
			cout << "Try Again!" << endl << endl;
		}
		catch (InvalidNumberException & s)
		{
			invalidOption = true;
			cout << s;
			cout << "Try Again!" << endl << endl;
		}

		cout << endl;

	} while (invalidOption);



	// HARD CODED FOR BASE PORTO
	// vector<string> areaOfInfluence = { "Porto", "Matosinhos", "Vila Nova de Gaia", "Gondomar", "Maia" };

	string newName;
	bool invalidName;

	Address newAddress;
	bool invalidAddress = false;
	string fullAddress;
	vector<string> areaOfInf = this->getAreaOfInfluence();

	bool invalidProduct;
	string strProduct;
	Product product;
	vector<Product*> productsVec;
	bool notEnded = true;

	set<string> cuisineTypes;

	switch (attributeChoice) {
		// Name
		case 1:
			do {
				try
				{
					invalidName = false;
					cout << "Current Name: " << restaurants.at(restaurantChoice)->get_name() << endl;
					cout << "Updated Name: ";
					getline(cin, newName);
					if (!isString(newName))
						throw InvalidStringException(newName);
					//if (cin.eof()) {
					//	cin.clear();
					//	return;
					//}

					cout << endl;
					restaurants.at(restaurantChoice)->setName(newName);
				}
				catch (InvalidStringException & s)
				{
					invalidName = true;
					cout << s;
					cout << "Try Again!" << endl << endl;
				}

			} while (invalidName);
			break;

		// Address
		case 2:
			do {
				invalidAddress = false;

				cout << "Current Address:\n" << restaurants.at(restaurantChoice)->get_address() << endl;
				cout << "Updated Address (District / Town / Street / No / Floor / Latitude / Longitude): " << endl;
				getline(cin, fullAddress);

				if (cin.eof()) {
					cin.clear();
					return;
				}

				try {
					bool teste = newAddress.parse(fullAddress);
					if (!teste)
						throw InvalidAddressException(fullAddress);

					// if it doesnt belong to the are of influence it is considered invalid
					if (find(areaOfInf.begin(), areaOfInf.end(), newAddress.get_town()) == areaOfInf.end()) {
						throw InvalidCityException(newAddress.get_town());
					}
				}

				catch (InvalidAddressException & a) {
					invalidAddress = true;
					cout << a;
					cout << "Try Again!" << endl << endl;
				}
				catch (InvalidCityException & c)
				{
					cout << c;
					cin.ignore();
					cout << ">>";
					return;
				}

				cout << endl;
			} while (invalidAddress);

			cout << endl;
			restaurants.at(restaurantChoice)->setAddress(newAddress);
			break;

		// Products
		case 3:
			cout << "Current List of Products:" << endl; //Implementar exceção para produtos

			for (auto & prod : restaurants.at(restaurantChoice)->get_products()) {
				cout << prod->get_name() << " : " << prod->get_cuisine_type() << " : " << prod->get_price() << endl;
			}

			cout << endl;
			cout << "Updated List of Products (Name : Type : Price), type 'done' to stop input" << endl;

			do {
				do {
					invalidProduct = false;

					getline(cin, strProduct);

					//if (cin.eof()) {
					//	cin.clear();
					//	return;
					//}

					if (strProduct == "done") {
						notEnded = false;
						break;
					}

					try {
						bool teste = product.parse(strProduct);
						if (!teste)
							throw InvalidProductException(strProduct);
						Product * productPtr = new Product;
						*productPtr = product;
						productsVec.push_back(productPtr);
					}
					catch (InvalidProductException & p) {
						invalidProduct = true;
						cout << p;
						cout << "Try Again!" << endl << endl;

					}
				} while (invalidProduct);
				
			} while (notEnded);
			restaurants.at(restaurantChoice)->setProducts(productsVec);

			// updating cuisine types
			
			for (auto & prod : restaurants.at(restaurantChoice)->get_products()) {
				cuisineTypes.insert(prod->get_cuisine_type());
			}

			
			restaurants.at(restaurantChoice)->setCuisineTypes(cuisineTypes);

			// updating price average
			restaurants.at(restaurantChoice)->setPriceAverage();

			break;
	}

	cout << endl;
	cout << "Restaurant successfully edited! (Enter to continue)" << endl;
	cout << ">> ";
	cin.ignore();
}

void Base::removeRestaurant() {

	vector<Restaurant*>::iterator it;
	bool invalidOption;
	string strChoice;
	int restaurantChoice;
	unsigned index = 0;

	cout << "Pick the restaurant you want to change information about:" << endl;

	do {
		index = 0;
		invalidOption = false;

		for (it = restaurants.begin(); it != restaurants.end(); ++it, ++index) {
			cout << index + 1 << ". " << (*it)->get_name() << endl;
		}
		try {
			cout << "0. Go Back" << endl;
			cout << ">> ";
			getline(cin, strChoice);

			if (strChoice == "0") {
				cin.clear();
				utils::clear_screen();
				return;
			}

			if (!isNumber(strChoice))
				throw InvalidNumberException(strChoice);
			restaurantChoice = stoi(strChoice);

			if (InvalidOptions(restaurants.size(), restaurantChoice)) {
				throw InvalidOptionException(restaurantChoice);
			}
		}
		catch (InvalidOptionException & o) {
			invalidOption = true;
			cout << o;
			cout << "Try Again!" << endl;
		}
		catch (InvalidNumberException & s)
		{
			invalidOption = true;
			cout << s;
			cout << "Try Again!" << endl << endl;
		}


		cout << endl;

	} while (invalidOption);

	restaurantChoice--;	// not to excede the max index available

	restaurants.erase(restaurants.begin() + restaurantChoice);

	cout << "Restaurant removed successfully" << endl;
	cout << ">> ";
	cin.ignore();
}


void Base::searchForRestaurant() 
{
	string name;
	bool retry = true;
	do
	{
		try
		{
			retry = false;
			cout << "Which Restaurant do you want?" << endl;
			getline(cin, name);
			bool notFound = true;
			//if (cin.eof())
			//{
			//	cin.clear();
			//	return;
			//}
			//sort(restaurants.begin(), restaurants.end(), sortRule<Restaurant *>);
			vector<Restaurant*>::iterator it;
			for (it = restaurants.begin(); it != restaurants.end(); it++) // Sequencial Search
			{
				if ((*it)->get_name() == name)
				{
					notFound = false;
				    cout << "Products Available" << endl;
					vector<Product*> vec = (*it)->get_products();
					vector<Product*>::iterator ite;
					for (ite = vec.begin(); ite != vec.end(); ite++)
					{
						cout << *(*ite);
						cout << endl;
					}
					cout << endl;
				}
			}
			if (notFound)
				throw RestaurantNotFoundException(name);
		}
		catch (RestaurantNotFoundException & r)
		{
			retry = true;
			cout << r;
			cout << "Try Again!" << endl << endl;
		}
		
	} while (retry);
	
}

void Base::searchForGeographicArea()
{
	bool retry = true;
	do
	{
		try
		{
			retry = false;
			string city;
			cout << "Which City do you want?" << endl;
			getline(cin, city);
			bool notFound = true;
			//if (cin.eof())
			//{
			//	cin.clear();
			//	return;
			//}
			vector<Restaurant*>::iterator it;
			for (it = restaurants.begin(); it != restaurants.end(); it++) // Sequencial Search
			{
				if ((*it)->get_address().get_town() == city)
				{
					notFound = false;
					cout << "Products Available" << endl << endl;
					cout << (*it)->get_name() << ": " << endl;
					vector<Product*> vec = (*it)->get_products();
					vector<Product*>::iterator ite;
					for (ite = vec.begin(); ite != vec.end(); ite++)
					{
						cout << *(*ite);
						cout << endl;
					}
					cout << endl;
				}
			}
			if (notFound)
				throw GeographicAreaNotFoundException(city);
		}
		catch (GeographicAreaNotFoundException & g)
		{
			retry = true;
			cout << g;
			cout << "Try Again!" << endl << endl;
		}
	} while (retry);
	
}

void Base::searchForPriceRange() // o price range não tá a ter casas decimais
{
	bool retry = true;
	do
	{
		try
		{
			retry = false;
			string min, max;
			double min_num, max_num;
			cout << "Which Price Range do you want?" << endl;
			cout << "Min: ";
			getline(cin, min);
			if (!isNumber(min))
				throw InvalidNumberException(min);
			cout << "Max: ";
			getline(cin, max);
			if (!isNumber(max))
				throw InvalidNumberException(max);
			min_num = stod(min);
			max_num = stod(max);
			if (max_num < min_num)
				throw InvalidPriceRangeException(min_num, max_num);
			cout << endl << "Products" << endl;
			bool none = true;
			vector<Restaurant*>::iterator it;
			for (it = restaurants.begin(); it != restaurants.end(); it++)
			{
				double price = (*it)->get_price_average();
				if (price >= min_num && price <= max_num)
				{
					none = false;
					cout << (*it)->get_name() << endl;
					vector<Product*> vec = (*it)->get_products();
					vector<Product*>::iterator ite;
					for (ite = vec.begin(); ite != vec.end(); ite++)
					{
						cout << *(*ite);
						cout << endl;
					}
				}
			}
			if (none)
				cout << "None" << endl;
		}
		catch (InvalidNumberException & n)
		{
			retry = true;
			cout << n;
			cout << "Try Again!" << endl << endl;
		}
		catch (InvalidPriceRangeException & p)
		{
			retry = true;
			cout << p;
			cout << "Try Again!" << endl << endl;
		}
		
	} while (retry);
	
}

void Base::searchForCuisineTypes()
{
	bool retry = true;
	do
	{
		try
		{
 			retry = false;
			string type;
			cout << "Which Cuisine Type do you want?" << endl;
			getline(cin, type);
			bool notFound = true;
			//if (cin.eof())
			//{
			//	cin.clear();
			//	return;
			//}
			vector<Restaurant*>::iterator it;
			for (it = restaurants.begin(); it != restaurants.end(); it++)
			{
				vector<Product*> vec = (*it)->get_products();
				vector<Product*>::iterator ite;
				int count = 0;
				for (ite = vec.begin(); ite != vec.end(); ite++)
				{
					if ((*ite)->get_cuisine_type() == type)
					{
						notFound = false;
						if (!count)
						{
							cout << (*it)->get_name() << ": " << endl;
							count++;
						}
						cout << *(*ite);
						cout << endl;
					}

				}

			}
			if (notFound)
				throw CuisineTypeNotFoundException(type);
		}
		catch (CuisineTypeNotFoundException & c)
		{
			retry = true;
			cout << c;
			cout << "Try Again!" << endl << endl;
		}
		
	} while (retry);
	
}

Delivery* Base::getDeliveryMan()
{
	//buscar o delivery man c menos historial feito
	vector<Delivery*> delivery_men;
	for (int i = 0; i < workers.size(); i++)
	{
		Delivery *d = dynamic_cast<Delivery *> (workers[i]);
		if (d != NULL)
		{
			delivery_men.push_back(d);
		}
	}
	size_t comp = delivery_men[0]->get_history().size();
	Delivery * result = delivery_men[0];
	vector<Delivery*>::iterator it;
	for (it = delivery_men.begin(); it != delivery_men.end(); it++)
	{
		if ((*it)->get_history().size() < comp)
		{
			comp = (*it)->get_history().size();
			result = (*it);
		}
	}
	return result;
}



void Base::writeRestaurantsFile(string fileName) {

	ofstream restFileInput("restaurants_p_copy.txt");

	if (!(restFileInput.is_open())) {
		// Need an Exception Here!
	}
	bool first = true;

	for (auto & restaurant : restaurants) {

		if (first) {
			first = false;
		}

		else {
			restFileInput << ";;;" << endl;
		}

		restFileInput << restaurant->get_name() << endl;
		restFileInput << restaurant->get_address() << endl;

		for (auto & product : restaurant->get_products()) {
			restFileInput << product->get_name();
			restFileInput << " : ";
			restFileInput << product->get_cuisine_type();
			restFileInput << " : ";
			restFileInput << product->get_price() << endl;
		}
	}
	restFileInput.close();
}

void Base::writeDeliveriesFile(string filename) {

	ofstream deliveriesFileInput("deliveries_p_copy_.txt");

	if (!(deliveriesFileInput.is_open())) {
		// Need an Exception Here!
	}

	bool firstOrder = true;
	bool firstProd = true;

	for (auto & order : orders) {
		if (firstOrder) {
			firstOrder = false;
		}
		else {
			deliveriesFileInput << "\n;;;" << endl;
		}
		deliveriesFileInput << order.second->getID() << endl;
		deliveriesFileInput << order.second->getRestaurant()->get_name() << endl;
		deliveriesFileInput << order.second->getDeliveryFee() << endl;
		deliveriesFileInput << order.second->getDeliver()->getInsuccessMessage() << endl; // missing reason 
		deliveriesFileInput << setw(2) << setfill('0') << order.second->getDateTime().getDay() << ":";
		deliveriesFileInput << setw(2) << setfill('0') << order.second->getDateTime().getMonth() << ":";
		deliveriesFileInput << order.second->getDateTime().getYear() << ":";
		deliveriesFileInput << setw(2) << setfill('0') << order.second->getDateTime().getHours() << ":";
		deliveriesFileInput << setw(2) << setfill('0') << order.second->getDateTime().getMinutes() << endl;

		deliveriesFileInput << setw(2) << setfill('0') << order.second->getDeliver()->getDateTime().getDay() << ":";
		deliveriesFileInput << setw(2) << setfill('0') << order.second->getDeliver()->getDateTime().getMonth() << ":";
		deliveriesFileInput << order.second->getDeliver()->getDateTime().getYear() << ":";
		deliveriesFileInput << setw(2) << setfill('0') << order.second->getDeliver()->getDateTime().getHours() << ":";
		deliveriesFileInput << setw(2) << setfill('0') << order.second->getDeliver()->getDateTime().getMinutes() << endl;

		firstProd = true;

		for (auto & prod : order.second->getProducts()) {
			if (firstProd) {
				firstProd = false;
			}
			else {
				deliveriesFileInput << endl;
			}
			deliveriesFileInput << prod->get_name();
			deliveriesFileInput << " : ";
			deliveriesFileInput << prod->get_cuisine_type();
			deliveriesFileInput << " : ";
			deliveriesFileInput << prod->get_price();
		}
	}
	deliveriesFileInput.close();
}

void Base::writeWorkersFile(string fileName) {

	ofstream workersFileInput("workers_p_copy.txt");

	if (!(workersFileInput.is_open())) {
		// Need an Exception Here!
	}

	bool foundFirstDelivery = false;
	bool firstWorker = true;
	bool firstOrder = true;
	
	Admin *adminCheck;
	Delivery *deliveryCheck;

	for (size_t i = 0; i < workers.size(); i++) {
		adminCheck = dynamic_cast<Admin*> (workers.at(i));
		if (adminCheck != NULL) {
			if (firstWorker) {
				firstWorker = false;
			}
			else {
				workersFileInput << ";" << endl;
			}
			workersFileInput << adminCheck->get_name() << endl;
			workersFileInput << adminCheck->get_NIF() << endl;
			workersFileInput << adminCheck->get_birthday() << endl;
			workersFileInput << adminCheck->get_wage() << endl;
			workersFileInput << adminCheck->get_role() << endl;

		}
		else {
			deliveryCheck = dynamic_cast<Delivery*> (workers.at(i));
			if (!foundFirstDelivery) {
				foundFirstDelivery = true;
				workersFileInput << ";;;" << endl;
			}
			else {
				workersFileInput << ";" << endl;
			}
			workersFileInput << deliveryCheck->get_name() << endl;
			workersFileInput << deliveryCheck->get_NIF() << endl;
			workersFileInput << deliveryCheck->get_birthday() << endl;
			workersFileInput << deliveryCheck->get_vehicle().get_brand();
			workersFileInput << " : ";
			workersFileInput << deliveryCheck->get_vehicle().get_type();
			workersFileInput << " : ";
			workersFileInput << deliveryCheck->get_vehicle().get_registration_date();
			workersFileInput << endl;

			// To "Fix" when orders.find is working correctly
			// cout << "history: " << deliveryCheck->get_history().size() << endl;

			if (deliveryCheck->get_history().size() == 0) {
				workersFileInput << "-";
			}

			else {
				for (auto & order : deliveryCheck->get_history()) {
					if (firstOrder) {
						firstOrder = false;
					}
					else {
						workersFileInput << " : ";
					}

					workersFileInput << order.second->getID();
				}
			}
			workersFileInput << endl;
		}
	}
	workersFileInput.close();
}

void Base::writeClientsFile(string fileName) {

	ofstream clientsFileInput("clients_p_copy.txt");

	if (!(clientsFileInput.is_open())) {
		// Need an Exception Here!
	}
	bool firstClient = true;
	bool firstOrder = true;

	for (auto & client : clients) {

		if (firstClient) {
			firstClient = false;
		}

		else {
			clientsFileInput << ";" << endl;
		}

		clientsFileInput << client->get_name() << endl;
		clientsFileInput << client->get_address() << endl;
		clientsFileInput << client->get_NIF() << endl;
		
		if (client->get_orders().size() == 0) {
			clientsFileInput << "-";
		}
		else {
			for (auto & order : client->get_orders()) {

				if (firstOrder) {
					firstOrder = false;
				}
				else {
					clientsFileInput << " : ";
				}

				clientsFileInput << order.second->getID();
			}
		}
		clientsFileInput << endl;
	}
	clientsFileInput.close();
}


//void Base::writeAll()
//{
	//writeRestaurantsFile(restaurantsFileName);

//	writeRestaurantsFile(restaurantsFileName);

//	writeDeliveriesFile(deliveriesFileName);
//	writeWorkersFile(workersFileName);
//	writeClientsFile(clientsFileName);
//}