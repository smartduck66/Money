//
// Book : Version du chapitre 9 de l'ouvrage.
// "Programming -- Principles and Practice Using C++" de Bjarne Stroustrup (2ème édition : 2014)
// Commit initial : 28 janvier 2018 
// Commit en cours : Exercice n°14 de la page 341
// Caractères spéciaux : [ ]   '\n'   {  }   ||   ~   _     @

// Librairie indispensable sous Windows et VC++
#include "stdafx.h"


// Librairie restreinte fournie par BS
#include "std_lib_facilities.h"

class Money {
public:
	enum Devise {
		USD = 0, EUR, GBP, CAD, INR, MXN, AUD, CNY, MYR, COP
	};
	
	// Constructeur par défaut
	Money() = default;

	// Constructeur
	Money(int a, Devise d) noexcept:money_amount(a), money_devise(d) {  }
	
	class Invalid { };                 // to throw as exception

	// non-modifying operations
	long int get_amount() const noexcept { return money_amount; }
	Devise get_devise() const noexcept { return money_devise; }


	// modifying operations
	// none ***************

private:							// Les variables privées sont initialisées par défaut : CPP CoreGuidelines
	long int money_amount = 0;
	Devise money_devise = USD;
	
};

double conversion_en_USD(const Money& somme)
{

	// Taux de change 1 US Dollar versus autres devises : 28 janvier 2018
	vector <double> const taux_change = { 1,	0.80475,	0.70644,	1.23094,	63.5940,	18.4901,	1.23244,	6.31977,	3.87150,	2808.99 };

	return somme.get_amount() / taux_change[int(somme.get_devise())];
	
}

int devise_saisie(string d)
{
	if (d == "USD")return 0;
	if (d == "EUR")return 1;
	if (d == "GBP")return 2;
	if (d == "CAD")return 3;
	if (d == "INR")return 4;
	if (d == "MXN")return 5;
	if (d == "AUD")return 6;
	if (d == "CNY")return 7;
	if (d == "MYR")return 8;
	if (d == "COP")return 9;
	
	// Le trigramme saisi ne correspond à aucune devise renseignée
	throw Money::Invalid();

}

ostream& operator<<(ostream& os, const Money& somme)
{

	// Libellés des devises
	vector <string> const nom_devise = { "US dollars","euros","livres sterling","dollars canadiens","roupies indiennes","pesos mexicains","dollars australiens","yuans chinois","ringgits malaisiens","pesos colombiens" };
		
	if (somme.get_devise() == int(Money::USD)) {
		return os << "La somme est de " + to_string(somme.get_amount()) + " " + nom_devise[somme.get_devise()];
		
	}
	else {
		return os << "La somme est de " + to_string(somme.get_amount()) + " " + nom_devise[somme.get_devise()]<<", ce qui correspond à " << defaultfloat << conversion_en_USD (somme)<<" $";
	}
	
}

istream& operator>>(istream& is, Money& somme)
{
	
	long int s;
	string d;
	char ch1;

	is >> s >> ch1 >> d;
	if (!is) return is;
		
	if (ch1 !=  ',' ) {									// oops: format error
		is.clear(ios_base::failbit);                    // set the fail bit
		return is;
	}
		
	somme = Money( s, Money::Devise(devise_saisie(d)) );     // update somme
	return is;

}

Money operator+(const Money& somme1, const Money& somme2)
{
	return Money(conversion_en_USD(somme1) + conversion_en_USD(somme2),Money::USD);
}


int main()
{
	try
	{
		// A mettre dans la 1ère ligne du "main" pour afficher les caractères accentués sous la console Windows
		std::locale::global(std::locale("fr-FR"));

		/*
		// Tests de la création et de l'affichage de sommes d'argent ********************************************************** OK
		// Création de sommes dans un vecteur et affichage
		vector <Money> somme{ };
		
		somme.push_back({ 100, Money::USD });
		somme.push_back({ 200,Money::GBP });
		somme.push_back({ 3000,Money::COP });
		somme.push_back({ 1000,Money::EUR });

		for (auto x : somme)
			cout << x << '\n';
		
		*/

		Money somme1{};
		Money somme2{};

		cout << "Saisissez les deux sommes d'argent à additionner sous le format NNNNNN, AAA (USD, EUR, GBP, CAD, INR, MXN, AUD, CNY, MYR, COP) :\n";
		cin >> somme1 >> somme2;
		
		// Addition des deux sommes
		cout << somme1 + somme2<< "\n";
		
		keep_window_open();

		return 0;
	}
	catch (Money::Invalid&)
	{
		cerr << "error: la devise saisie est inconnue ! \n";
		keep_window_open("q");	// For some Windows(tm) setup
		return 1;
	}


	catch (runtime_error& e)
	{	// this code is to produce error messages
		cout << e.what() << '\n';
		keep_window_open("q");	// For some Windows(tm) setups
	}
}

