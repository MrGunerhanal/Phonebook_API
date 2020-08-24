#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <set>

using namespace std;
using namespace std::rel_ops;

// Question 1
// Base class person with virtual member functions so we can override them later
class person
{
  protected:
    string _name;
    string _surname;

  public:
    person() : _name("NoName"), _surname("Unknown"){};
    person(string _n) : _name(_n), _surname("Unknown"){};
    person(string _n, string _s) : _name(_n), _surname(_s) {}

    virtual string name() const
    {
        return _name;
    }

    virtual string surname() const
    {
        return _surname;
    }

    virtual string telephone() const
    {
        return "";
    }

    virtual string email() const
    {
        return "";
    }

    virtual bool has_telephone_p() const
    {
        return false;
    }
    virtual bool has_email_p() const
    {
        return false;
    }
};

// Question 2
class person_with_telephone : public virtual person
{
  protected:
    string _telephone;

  public:
    person_with_telephone(string _n, string _s, string _t) : person(_n, _s), _telephone(_t){};

    virtual string telephone() const
    {
        return _telephone;
    }

    void set_telephone(const string _t)
    {
        _telephone = _t;
    }

    virtual bool has_telephone_p() const
    {
        if (_telephone != "")
        {
            return true;
        }
        return false;
    }
};

// Question 3
class person_with_email : public virtual person
{
  protected:
    string _email;

  public:
    person_with_email(string _n, string _s, string _e) : person(_n, _s), _email(_e){};

    virtual string email() const
    {
        return _email;
    }

    void set_email(const string _e)
    {
        _email = _e;
    }

    virtual bool has_email_p() const
    {
        if (_email != "")
        {
            return true;
        }
        return false;
    }
};

// Question 4
class person_with_telephone_and_email : public person_with_telephone, public person_with_email
{
  public:
    person_with_telephone_and_email(string _n, string _s, string _t, string _e) : person(_n, _s), person_with_telephone(_n, _s, _t), person_with_email(_n, _s, _e){};

    // Solving ambiguity since the following methods exists in both of the inherited classes
    virtual bool has_telephone_p() const
    {
        return person_with_telephone::has_telephone_p();
    }

    virtual bool has_email_p() const
    {
        return person_with_email::has_email_p();
    }
};

// Question 5
// Overriding the << operator to output person class to the following
// <person S " surname " N " name [" T " telephone]? [" E " email]? " >
ostream &operator<<(ostream &out, const person &p)
{
    out << "<person S " << p.surname() << " N " << p.name();

    if (p.has_telephone_p())
    {
        out << " T " << p.telephone();
    }

    if (p.has_email_p())
    {
        out << " E " << p.email();
    }

    out << " >";

    return out;
}

/**
 / Question 6
 / Format: <person S surname N name T telephone E email >
 / we pass Person as pointer because we can store inherited classes into it
 / based on the input parameters
 / We used a while loop and the advantage is that user can enter the above tag in any order
 / for example <person E email N name T telephone S surname > works too!
 */
istream &read_person(istream &in, person *&p)
{
    string param, _surname, _name, _telephone, _email;
    char key;
    if ((in >> key >> param) && key == '<' && param == "person")
    {
        while (key != '>')
        {
            if ((in >> key) && key != '>')
            {
                switch (key)
                {
                case 'S':
                    in >> _surname;
                    break;
                case 'N':
                    in >> _name;
                    break;
                case 'T':
                    in >> _telephone;
                    break;
                case 'E':
                    in >> _email;
                    break;
                }
            }
        }

        // Construct the class into the given argument pointer based on the parameters
        if (_email != "" && _telephone != "")
        {
            p = new person_with_telephone_and_email(_name, _surname, _telephone, _email);
        }
        else if (_email != "")
        {
            p = new person_with_email(_name, _surname, _email);
        }
        else if (_telephone != "")
        {
            p = new person_with_telephone(_name, _surname, _telephone);
        }
        else
        {
            p = new person(_name, _surname);
        }
    }
    else
    {
        in.setstate(ios::badbit);
    }
    return in;
}

// Question 7
// Returns true if both name and surnames are equal
bool operator==(const person &p1, const person &p2)
{
    return p1.name() == p2.name() && p1.surname() == p2.surname();
}

// Compares 2 persons by surname and firstname alphabetically
bool operator<(const person &p1, const person &p2)
{
    if (p1.surname() != p2.surname())
    {
        return p1.surname() < p2.surname();
    }

    // When surnames are equal then we compare first names
    if (p1.name() != p2.name())
    {
        return p1.name() < p2.name();
    }

    // They are equal
    return false;
}

class contacts
{
  private:
    vector<person *> directory;

  public:
    void add(person *p)
    {
        for (auto l = begin(directory); l != end(directory); ++l)
        {
            if (*p == **l)
            {
                (*l) = p;
                return;
            }
        }
        directory.push_back(p);
    }

    /**
     / Question 8
     / search in the directory to find a person with matching name or surname to keyword value
     */
    vector<person *> find(const string &keyword)
    {
        vector<person *> result;
        for_each(begin(directory), end(directory), [&result, &keyword](person *p) {
            // If first name or last name matches
            if (p->name() == keyword || p->surname() == keyword)
            {
                result.push_back(p);
            }
        });
        return result;
    }

    /**
     / Question 9
     / take the output stream as argument
     / sort the directory and appends it into the output stream and return it
     */
    ostream &in_order(ostream &out)
    {
        sort(begin(directory), end(directory),
             [](person *p, person *d) {
                 return *p < *d;
             });
        for_each(begin(directory), end(directory),
                 [&out](person *p) {
                     out << *p << endl;
                 });
        return out;
    }

    /**
     / Question 10
     / same as above but it only adds the entries with a phone number to the output stream
     */
    ostream &with_telephone(ostream &out)
    {
        sort(begin(directory), end(directory),
             [](person *p, person *d) {
                 return *p < *d;
             });

        for_each(begin(directory), end(directory),
                 [&out](person *p) {
                     if (p->has_telephone_p())
                         out << *p << endl;
                 });
        return out;
    }

    void print()
    {
        for (auto l = begin(directory); l != end(directory); ++l)
        {
            cout << **l << endl;
        }
    }
};

// overriding << operator for vector<person *>
// Used to output the find member function result
ostream &operator<<(ostream &out, const vector<person *> _persons)
{
    for (auto l = begin(_persons); l != end(_persons); ++l)
    {
        out << **l << endl;
    }
    return out;
}

int main()
{
    cout << "\nQuestion 1:" << endl;
    person p1;
    cout << p1 << endl;

    person p2("Burak");
    cout << p2 << endl;

    person p3("Majid", "Khosravi");
    cout << p3 << endl;

    cout << "\nQuestion 2:" << endl;

    person_with_telephone p4("Will", "Smith", "0752555555");
    cout << p4 << endl;

    cout << "\nQuestion 3:" << endl;
    person_with_email p5("Brad", "Pitt", "brad@pitt.com");
    cout << p5 << endl;

    cout << "\nQuestion 4:" << endl;
    person_with_telephone_and_email p6("George", "Clooney", "06666666", "george@clooney.com");
    cout << p6 << endl;

    cout << "\nQuestion 5 ( Tested above using << operator ) ." << endl;

    cout << "\nQuestion 6" << endl;
    cout << "Enter details in the following format (Ctrl+D to continue): " << endl;
    cout << "<person S [surname] N [name]  T [telephone]? E [email]? >" << endl;

    person *pp = 0;
    while (read_person(cin, pp) && pp)
        cout << "Result: " << *pp << endl;

    cout << "\nQuestion 7 - Testing replace functionality" << endl;
    person_with_telephone_and_email p7("Majid", "Khosravi", "07777777", "majid.khosravi@city.ac.uk");
    cout << p7 << endl;

    cout << "\nQuestion 7 - Testing the contacts class" << endl;
    contacts phonebook;
    phonebook.add(&p1);
    phonebook.add(&p2);
    phonebook.add(&p3);
    phonebook.add(&p4);
    phonebook.add(&p5);
    phonebook.add(&p6);
    phonebook.add(&p7);
    phonebook.print();

    cout << "\nQuestion 8" << endl;
    cout << "Searching for Majid: " << endl;
    cout << phonebook.find("Majid") << endl;

    cout << "\nQuestion 9" << endl;
    cout << "Sorting: " << endl;
    phonebook.in_order(cout) << endl;

    cout << "\nQuestion 10" << endl;
    cout << "Sorting and showing with telephone only: " << endl;
    phonebook.with_telephone(cout) << endl;

    return 0;
}