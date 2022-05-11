// test_main.cpp
#define BOOST_TEST_DYN_LINK 1
#define BOOST_TEST_MODULE TableTest
#include <boost/test/included/unit_test.hpp>
#include <vector>
#include <string>
#include "../src/storage/Table.h"
#include "../src/storage/PrimeStream.h"
#include "../src/storage/TimeMeasure.h"
#include <algorithm>
class TestItem{
	public:
		TestItem(){}
		TestItem(std::string n,std::string i) : name(n),interface(i){}
		std::string name;
		std::string interface;
};

BOOST_AUTO_TEST_SUITE(TableTestSuite)

BOOST_AUTO_TEST_CASE(COMPLEX_ITEM_TEST){
    std::cout<<"COMPLEX_ITEM_TEST"<<std::endl;
	Table<std::string,TestItem> table;
	table["test"] = TestItem("Hallo","Welt");
	BOOST_CHECK(table.lookup("test"));
	
}


BOOST_AUTO_TEST_CASE(VECTOR_ITEM_TEST){
    std::cout<<"VECTOR_ITEM_TEST"<<std::endl;
    Table<std::string,std::vector<TestItem>> table;
	std::vector<TestItem> v;
	v.push_back(TestItem("HALLO","WELT"));
	table["test"] = v;
	table["test"].push_back(TestItem("TestItem","2"));
	table["test"].push_back(TestItem("TestItem","3"));
	BOOST_CHECK(table.lookup("test"));

}


BOOST_AUTO_TEST_CASE(ConstructionCase)
{
    std::cout<<"ConstructionCase"<<std::endl;
    Table<std::string,int> table;
    BOOST_CHECK(table.getSizeOfFirstContainer() == 1009);
    BOOST_CHECK(table.getSizeOfSecondContainer()== 1013);

    Table<std::string,int,1,1> t2;
    BOOST_CHECK(t2.getSizeOfFirstContainer() == 1);
    BOOST_CHECK(t2.getSizeOfSecondContainer()== 1);

}

BOOST_AUTO_TEST_CASE(AddScalarTypeCase)
{
    std::cout<<"AddScalarTypeCase"<<std::endl;
    Table<std::string,int> table;
    table["first"] = 12389;
    table["second"]= 12390;
    BOOST_CHECK(table["first"] == 12389);
    BOOST_CHECK(table["second"] == 12390);
    BOOST_CHECK(table["third"] != 12391);
    BOOST_CHECK(table["third"] == 0);
}

BOOST_AUTO_TEST_CASE(AddScalarPtrTypeCase)
{
    std::cout<<"AddScalarPtrTypeCase"<<std::endl;
    Table<std::string,int*> table;
    int * foo = new int[2];
    foo[0] = 12389;
    foo[1] = 12390;
    table["first"] = foo;
    BOOST_CHECK(table["first"][0] == 12389);
    BOOST_CHECK(table["first"][1] == 12390);
    delete [] foo;
}

BOOST_AUTO_TEST_CASE(DestructionScalarPtrTypeCase)
{
    std::cout<<"DestructionScalarPtrTypeCase"<<std::endl;
    auto *table = new Table<std::string,int*>();
    int * foo = new int[2];
    foo[0] = 12389;
    foo[1] = 12390;
    table->addItem("first", foo);
    delete table;
    delete [] foo;
}

BOOST_AUTO_TEST_CASE(LookupCase){
    std::cout<<"LookupCase"<<std::endl;
    Table<std::string,int> table;
    table["first"] = 12389;
    table["second"]= 12390;
    BOOST_CHECK(table["first"] == 12389);
    BOOST_CHECK(table["second"] == 12390);
    BOOST_CHECK(table.lookup("third")==false);
    BOOST_CHECK(table["third"] == 0);
    BOOST_CHECK(table.lookup("third"));
}

BOOST_AUTO_TEST_CASE(RemoveCase){
    std::cout<<"RemoveCase"<<std::endl;
    Table<std::string,int> table;
    table["first"] = 12389;
    table["second"]= 12390;
    BOOST_CHECK(table["first"] == 12389);
    BOOST_CHECK(table["second"] == 12390);
    BOOST_CHECK(table.lookup("third")==false);
    BOOST_CHECK(table["third"] == 0);
    BOOST_CHECK(table.lookup("third"));
    table.remove("third");
    BOOST_CHECK(table.lookup("third")==false);
    table.remove("michael");
}

BOOST_AUTO_TEST_CASE(RemoveAll){
    std::cout<<"RemoveAll"<<std::endl;
    Table<int,int> table;
	for(int i = 0; i < 1000000; ++i){
		table[i] = i;
	}
	for(int i = 0; i < 1000000; ++i){
		BOOST_CHECK(table.lookup(i));
	}
	table.removeAll();
	for(int i = 0; i < 1000000; ++i){
		BOOST_CHECK(table.lookup(i)== false);
	}

}




BOOST_AUTO_TEST_CASE(RemoveAllCase){
    std::cout<<"RemoveAllCase"<<std::endl;
    Table<std::string,int> table;
    table["first"] = 12389;
    table["second"]= 12390;
    BOOST_CHECK(table["first"] == 12389);
    BOOST_CHECK(table["second"] == 12390);
    BOOST_CHECK(table.lookup("third")==false);
    BOOST_CHECK(table["third"] == 0);
    BOOST_CHECK(table.lookup("third"));
    table.removeAll();
    BOOST_CHECK(table.lookup("first")==false);
    BOOST_CHECK(table.lookup("second")==false);
    BOOST_CHECK(table.lookup("third")==false);
    table.removeAll();
}

BOOST_AUTO_TEST_CASE(AssignmentCase){
    std::cout<<"AssignmentCase"<<std::endl;
    Table<std::string,int> table;
    table["first"] = 12389;
    table["second"]= 12390;
    
    Table<std::string,int> cpyTable = table;

    BOOST_CHECK(cpyTable["first"] == 12389);
    BOOST_CHECK(cpyTable["second"] == 12390);

}

BOOST_AUTO_TEST_CASE(CopyCase){
    std::cout<<"CopyCase"<<std::endl;
    Table<std::string,int> table;
    table["first"] = 12389;
    table["second"]= 12390;
    
    Table<std::string,int> cpyTable(table);

    BOOST_CHECK(cpyTable["first"] == 12389);
    BOOST_CHECK(cpyTable["second"] == 12390);

}

BOOST_AUTO_TEST_CASE(MassiveInsertToTableCase){
    std::cout<<"MassiveInsertToTableCase"<<std::endl;
    Table<int,int> table;
   	for(std::size_t i  = 0; i < 1200000; ++i){
		table[i]= i;
    	}
  	for(std::size_t i  = 0; i < 1200000; ++i){
       		BOOST_CHECK(table[i] ==  i);
    	}
    
}

BOOST_AUTO_TEST_CASE(ZeroInsertionTest){
    std::cout<<"ZeroInsertionTest"<<std::endl;
    Table<int,int> table;
	BOOST_CHECK(table.lookup(0)==false);
	table[0] = 0;
	BOOST_CHECK(table.lookup(0));
	BOOST_CHECK(table[0] == 0 );

}




BOOST_AUTO_TEST_CASE(Massive_CROSS_LOOKUP_TEST){
    std::cout<<"Massive_CROSS_LOOKUP_TEST"<<std::endl;
    for(int k = 1; k < 100; ++k)
	{
		Table<int,int,5,3,PrimeStream> table;
		for(int i = 0; i < k; ++i){
			table[i] = i;
			for(int j = 0; j <= i; ++j){
				BOOST_CHECK_MESSAGE(table.lookup(j),"MiSSING KEY j:"
						+ std::to_string(j) 
						+" AFTER INSERT "+std::to_string(i)
						+" In Loop " + std::to_string(k));
			}
		}
	}

}


BOOST_AUTO_TEST_CASE(REHASH_TEST){
    std::cout<<"REHASH_TEST"<<std::endl;
    Table<int,int,3,2,PrimeStream> table;
	table[0] = 0;
	table[1] = 1;
	table[2] = 2;
	table[3] = 3;
	table[4] = 4;
	table[5] = 5;
	BOOST_CHECK(table.lookup(0));
	BOOST_CHECK(table.lookup(1));
	BOOST_CHECK(table.lookup(2));
	BOOST_CHECK(table.lookup(3));
	BOOST_CHECK(table.lookup(4));
	BOOST_CHECK(table.lookup(5));
}

BOOST_AUTO_TEST_CASE(MassiveLockupTest){
    std::cout<<"MassiveLockupTest"<<std::endl;
    Table<int,int> table;
	int saveIndex = 0;
	int secondIndex = 0;
   	for(std::size_t i  = 0; i < 1200000; ++i){
		table[i] = i;
		BOOST_CHECK(table.lookup(0));
	}
   	for(std::size_t i  = 0; i < 1200000; ++i){
		BOOST_CHECK(table.lookup(i));
	}
	BOOST_CHECK(table.lookup(0));
	
}

BOOST_AUTO_TEST_CASE(MassiveInsertTest){
    std::cout<<"MassiveInsertTest"<<std::endl;
    Table<int,int> table;
	int saveIndex = 0;
	int secondIndex = 0;
   	for(std::size_t i  = 0; i < 1200000; ++i){
		table.insert(i,i);
		BOOST_CHECK(table.lookup(0));
	}
   	for(std::size_t i  = 0; i < 1200000; ++i){
		BOOST_CHECK(table.lookup(i));
	}
	BOOST_CHECK(table.lookup(0));
	
}

BOOST_AUTO_TEST_CASE(ComplexItemInsertTest){
    std::cout<<"ComplexItemInsertTest"<<std::endl;
    Table<std::string,std::string> table;
    for(unsigned int i = 0; i <12000; ++i){
        table.insert(std::to_string(i),std::move(std::string("TEST") + std::to_string(i) ));
        int v = static_cast<int>((static_cast<double>(i)/12000) *100);
        if(  v % 5 == 0) {
            std::cout << "Inserted: " << v<<"%"<<std::endl;
        }
    }
    for(unsigned int i = 0; i <12000; ++i){
        BOOST_CHECK(table.lookup(std::to_string(i)));
        BOOST_CHECK(table[std::to_string(i)] == std::string("TEST") + std::to_string(i) );
    }
}


BOOST_AUTO_TEST_CASE(MoveOperatorTest){
    std::cout<<"MoveOperatorTest"<<std::endl;
    Table<int,int> table;
	for(std::size_t i  = 0; i < 1200000; ++i){
		table.insert(i,i);
		BOOST_CHECK(table.lookup(0));
	}
	Table<int,int> movedTable = std::move(table);
	for(std::size_t i  = 0; i < 1200000; ++i){
		BOOST_CHECK(movedTable.lookup(i));
	}
	BOOST_CHECK(movedTable.lookup(0));

}


BOOST_AUTO_TEST_CASE(ZERO_FAILURE_TEST){
    std::cout<<"ZERO_FAILURE_TEST"<<std::endl;
    Table<int,int> table;
	int a = 2022;
        int b = 2023;	
	table[0] = 0;

	table[a] = a;
	BOOST_CHECK(table.lookup(a));
	BOOST_CHECK(table.lookup(0));
}
BOOST_AUTO_TEST_CASE(FilterTestCase){
    std::cout<<"FilterTestCase"<<std::endl;
    Table<int,int> table;
   	for(std::size_t i  = 0; i < 1200000; ++i){
        	table[i]= i;
		if(table.lookup(0) == false){
			BOOST_CHECK(false);
		}
    }
	std::list<int> filteredList = table.filter( [&](const int & item){ return item <= 100; } );
	std::size_t i = 0;
	filteredList.sort();
	for(auto item : filteredList){
		BOOST_CHECK(item == i);
		i = i+1;
	}

}

BOOST_AUTO_TEST_CASE(RVALUE_REFECRENCE_OPERATOR_GET_SET_TEST){
    std::cout<<"RVALUE_REFECRENCE_OPERATOR_GET_SET_TEST"<<std::endl;
	measure<10000,std::chrono::microseconds> m;

	m.method_to_compare("RVALUE",[](){
		std::string x ="hallo_mein_name_ist_michael_burzan_und_wir_leben_in_einem";
		Table<std::string,int> table;
   		table[std::move(x)] = 1;
	});
	m.method_to_compare("REF",[](){
		std::string x ="hallo_mein_name_ist_michael_burzan_und_wir_leben_in_einem";
		Table<std::string,int> table;
   		table[x] = 1;
	});
	std::cout<<m<<"\n";

	
}

BOOST_AUTO_TEST_CASE(SPEED_UP_INSERT_TEST){
    std::cout<<"SPEED_UP_INSERT_TEST"<<std::endl;
	measure<10000,std::chrono::microseconds> m;

	m.method_to_compare("RVALUE_INSERT",[](){
		std::string x ="hallo_mein_name_ist_michael_burzan_und_wir_leben_in_einem";
		std::string y ="hallo_mein_name_ist_michael_burzan_und_wir_leben_in_einem";
		Table<std::string,std::string> table;
   		table.insert(x,std::move(y));
	});
	m.method_to_compare("REF_INSERT",[](){
		std::string x ="hallo_mein_name_ist_michael_burzan_und_wir_leben_in_einem";
		std::string y ="hallo_mein_name_ist_michael_burzan_und_wir_leben_in_einem";
		Table<std::string,std::string> table;
   		table[x] = y;
	});
	std::cout<<m<<"\n";

	
}



BOOST_AUTO_TEST_SUITE_END()
