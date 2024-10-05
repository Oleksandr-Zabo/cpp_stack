#include <iostream>
#include <string>
#include <Windows.h>
using namespace std;

HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);//to colors output

enum Color {
	Strng_color = 14, //yellow
	Error_color = 12, //red
	Error_color2 = 4, //dark_red
	Success_color = 10, //green
	Standart_color = 7 //white
};

template<typename T>
class Node {
private:
    T* _data = nullptr;
    int* _amount_of_data = nullptr;

    void delete_data()
    {
        if (_amount_of_data == nullptr)
        {
            if (_data != nullptr)
                delete _data;
        }
        else
        {
            if (_data != nullptr)
                delete[] _data;
            delete _amount_of_data;
        }
    }
public:
    Node() = default;
    Node(T data)
    {
        _data = new T{ data };
    }
    Node(T* data, int size)
    {
        _amount_of_data = new int{ size };
    }
    Node(const Node& other)
    {
        _data = new T{ *other._data };
        _amount_of_data = new int{ *other._amount_of_data };
    }

    void change_data(T new_value)
    {
        delete_data();
        _data = new T{ new_value };
    }

    void change_data(T* new_value, int size)
    {
        delete_data();

        _amount_of_data = new int{ size };
        _data = new T[size];

        for (int i = 0; i < size; i++)
        {
            _data[i] = new_value[i];
        }
    }

    T* get_data()
    {
        return _data;
    }

    int get_amount_of_data()
    {
        return *_amount_of_data;
    }

    ~Node()
    {
        delete_data();
    }
};

template<typename T>
class Stack
{
private:
    Node<T>** _items = nullptr;
    const int _step = 10;
    int _size;
    int _fact_size = 0;

    void check_resize()
    {
        if (_fact_size == _size - 1)
        {
            resize();
        }
    }

    void resize()
    {
        Node<T>** new_items = new Node<T>*[_size + _step];
        for (int i = 0; i < _size; i++)
        {
            new_items[i] = _items[i];
        }
        delete[] _items;
        _items = new_items;
        _size += _step;
    }
public:
    Stack()
    {
        _size = _step;
        _items = new Node<T>*[_size];
    }

    void push(T value)
    {
        check_resize();
        Node<T>* new_node = new Node<T>(value);
        _items[_fact_size] = new_node;
        _fact_size++;
    }

    void push(T* value, int size)
    {
        check_resize();
        Node<T>* new_node = new Node<T>(value, size);
        _items[_fact_size] = new_node;
        _fact_size++;
    }

    T pop()
    {
        if (_fact_size == 0)
        {
            throw std::out_of_range("Stack is empty");
        }
        Node<T>* item = _items[_fact_size - 1];
        T* data = item->get_data();
        T result = *data;
        delete item;
        _fact_size--;
        return result;
    }

    T top()
    {
        if (_fact_size == 0)
        {
            throw std::out_of_range("Stack is empty");
        }
        Node<T>* item = _items[_fact_size - 1];
        T* data = item->get_data();
        T result = *data;
        return result;
    }

    bool isEmpty() const
    {
        return _fact_size == 0;
    }

    ~Stack()
    {
        delete[] _items;
    }
};

bool is_balanced(const string& str) {
    Stack<char> stack;

    for (size_t i = 0; i < str.length(); ++i) {
        char ch = str[i];

        if (ch == '(' || ch == '{' || ch == '[') {
            stack.push(ch);
        }
        else if (ch == ')' || ch == '}' || ch == ']') {
            if (stack.isEmpty()) {
                SetConsoleTextAttribute(hConsole, Color::Error_color);
                cout << "Error: extra closing bracket '" << ch << "' at position " << i << "\n";

				SetConsoleTextAttribute(hConsole, Color::Strng_color);
                cout << str.substr(0, i + 1) << "\n";// substr(0, i + 1) - returns the substring from the beginning to the i-th character

                SetConsoleTextAttribute(hConsole, Color::Standart_color);
                return false;
            }
            char top = stack.pop();
            if ((ch == ')' && top != '(') || (ch == '}' && top != '{') || (ch == ']' && top != '[')) {
                SetConsoleTextAttribute(hConsole, Color::Error_color);
                cout << "Error: mismatched brackets '" << top << "' and '" << ch << "' at position " << i << "\n";
                
                SetConsoleTextAttribute(hConsole, Color::Strng_color);
                cout << str.substr(0, i + 1) << "\n";

                SetConsoleTextAttribute(hConsole, Color::Standart_color);
                return false;
            }
        }
    }


    if (!stack.isEmpty()) {
        SetConsoleTextAttribute(hConsole, Color::Error_color);
        cout << "Error: extra opening bracket '" << stack.pop() << "'\n";
        
        SetConsoleTextAttribute(hConsole, Color::Strng_color);
        cout << str << "\n";

        SetConsoleTextAttribute(hConsole, Color::Standart_color);
        return false;
    }

    if (str.back() != ';') {// back() - returns the last character of the string
        SetConsoleTextAttribute(hConsole, Color::Error_color);
        cout << "Error: the string must end with a ';' character\n";

        SetConsoleTextAttribute(hConsole, Color::Strng_color);
        cout << str.substr(0, str.length() - 1) << "\n";

        SetConsoleTextAttribute(hConsole, Color::Standart_color);
        return false;
    }
    return true;
}

void print_balanced(const string& str) {
	
    cout << "String: ";
	SetConsoleTextAttribute(hConsole, Color::Strng_color);

    cout << str << endl;


	if (is_balanced(str)) {
        SetConsoleTextAttribute(hConsole, Color::Success_color);
		cout << "The string is balanced\n";
	}
	else {
		SetConsoleTextAttribute(hConsole, Color::Error_color2);
		cout << "The string is not balanced\n";
	}
    cout << endl;

	SetConsoleTextAttribute(hConsole, Color::Standart_color);
}

int main() {
    

    string str = "[{x-y-z} * [x + 2y] - (z + 4x)];";
	print_balanced(str);

    string str2 = "[{x-y-z} * [x + 2y] - (z + 4x);";
    print_balanced(str2);

    string str3 = "([x-y-z} * [x + 2y) - {z + 4x)];";
    print_balanced(str3);

    string str4 = "[{x-y-z} * [x + 2y] - (z + 4x)]";
    print_balanced(str4);

	system("pause");
    return 0;
}
