// test_main.cpp
#define BOOST_TEST_DYN_LINK 1
#define BOOST_TEST_MODULE TableTest
#include <boost/test/included/unit_test.hpp>
#include <vector>
#include <string>
#include "../src/datastructure/table.h"
#include "../src/api/prime_stream.h"
#include "../src/api/time_measure.h"
#include <algorithm>
#include <utility>
#include "../src/api/macro_helper.h"
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
	table<std::string,TestItem> table;
	table["test"] = TestItem("Hallo","Welt");
	BOOST_CHECK(table.lookup("test"));
	
}


BOOST_AUTO_TEST_CASE(VECTOR_ITEM_TEST){
    std::cout<<"VECTOR_ITEM_TEST"<<std::endl;
    table<std::string,std::vector<TestItem>> table;
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
    table<std::string,int> table_t;
    BOOST_CHECK(table_t.get_size_of_first_container() == 1009);
    BOOST_CHECK(table_t.get_size_of_second_container() == 1013);

    table<std::string,int,1,1> t2;
    BOOST_CHECK(t2.get_size_of_first_container() == 1);
    BOOST_CHECK(t2.get_size_of_second_container() == 1);

}

BOOST_AUTO_TEST_CASE(AddScalarTypeCase)
{
    std::cout<<"AddScalarTypeCase"<<std::endl;
    table<std::string,int> t1;
    t1["first"] = 12389;
    t1["second"]= 12390;
    BOOST_CHECK(t1["first"] == 12389);
    BOOST_CHECK(t1["second"] == 12390);
    BOOST_CHECK(t1["third"] != 12391);
    BOOST_CHECK(t1["third"] == 0);
}

BOOST_AUTO_TEST_CASE(AddScalarPtrTypeCase)
{
    std::cout<<"AddScalarPtrTypeCase"<<std::endl;
    table<std::string,int*> t1;
    int * foo = new int[2];
    foo[0] = 12389;
    foo[1] = 12390;
    t1["first"] = foo;
    BOOST_CHECK(t1["first"][0] == 12389);
    BOOST_CHECK(t1["first"][1] == 12390);
    delete [] foo;
}

BOOST_AUTO_TEST_CASE(DestructionScalarPtrTypeCase)
{
    std::cout<<"DestructionScalarPtrTypeCase"<<std::endl;
    auto *t1 = new table<std::string,int*>();
    int * foo = new int[2];
    foo[0] = 12389;
    foo[1] = 12390;
    t1->add_item("first", foo);
    delete t1;
    delete [] foo;
}

BOOST_AUTO_TEST_CASE(LookupCase){
    std::cout<<"LookupCase"<<std::endl;
    table<std::string,int> t1;
    t1["first"] = 12389;
    t1["second"]= 12390;
    BOOST_CHECK(t1["first"] == 12389);
    BOOST_CHECK(t1["second"] == 12390);
    BOOST_CHECK(t1.lookup("third") == false);
    BOOST_CHECK(t1["third"] == 0);
    BOOST_CHECK(t1.lookup("third"));
}

BOOST_AUTO_TEST_CASE(RemoveCase){
    std::cout<<"RemoveCase"<<std::endl;
    table<std::string,int> t1;
        t1["first"] = 12389;
        t1["second"]= 12390;
    BOOST_CHECK(t1["first"] == 12389);
    BOOST_CHECK(t1["second"] == 12390);
    BOOST_CHECK(t1.lookup("third") == false);
    BOOST_CHECK(t1["third"] == 0);
    BOOST_CHECK(t1.lookup("third"));
    t1.remove("third");
    BOOST_CHECK(t1.lookup("third") == false);
    t1.remove("michael");
}

BOOST_AUTO_TEST_CASE(RemoveAll){
    std::cout<<"RemoveAll"<<std::endl;
    table<int,int> t1;
	for(int i = 0; i < 1000000; ++i){
        t1[i] = i;
	}
	for(int i = 0; i < 1000000; ++i){
		BOOST_CHECK(t1.lookup(i));
	}
		t1.remove_all();
	for(int i = 0; i < 1000000; ++i){
		BOOST_CHECK(t1.lookup(i) == false);
	}

}




BOOST_AUTO_TEST_CASE(RemoveAllCase){
    std::cout<<"RemoveAllCase"<<std::endl;
    table<std::string,int> t1;
        t1["first"] = 12389;
        t1["second"]= 12390;
    BOOST_CHECK(t1["first"] == 12389);
    BOOST_CHECK(t1["second"] == 12390);
    BOOST_CHECK(t1.lookup("third") == false);
    BOOST_CHECK(t1["third"] == 0);
    BOOST_CHECK(t1.lookup("third"));
		t1.remove_all();
    BOOST_CHECK(t1.lookup("first") == false);
    BOOST_CHECK(t1.lookup("second") == false);
    BOOST_CHECK(t1.lookup("third") == false);
		t1.remove_all();
}

BOOST_AUTO_TEST_CASE(AssignmentCase){
    std::cout<<"AssignmentCase"<<std::endl;
    table<std::string,int> t1;
        t1["first"] = 12389;
        t1["second"]= 12390;
    
    table<std::string,int> cpyTable = t1;

    BOOST_CHECK(cpyTable["first"] == 12389);
    BOOST_CHECK(cpyTable["second"] == 12390);

}

BOOST_AUTO_TEST_CASE(CopyCase){
    std::cout<<"CopyCase"<<std::endl;
    table<std::string,int> t1;
        t1["first"] = 12389;
        t1["second"]= 12390;
    
    table<std::string,int> cpyTable(t1);

    BOOST_CHECK(cpyTable["first"] == 12389);
    BOOST_CHECK(cpyTable["second"] == 12390);

}

BOOST_AUTO_TEST_CASE(MassiveInsertToTableCase){
    std::cout<<"MassiveInsertToTableCase"<<std::endl;
    table<int,int> t1;
   	for(int i  = 0; i < 1200000; ++i){
        t1[i]= i;
    	}
  	for(int i  = 0; i < 1200000; ++i){
       		BOOST_CHECK(t1[i] == i);
    	}
    
}

BOOST_AUTO_TEST_CASE(ZeroInsertionTest){
    std::cout<<"ZeroInsertionTest"<<std::endl;
    table<int,int> t1;
	BOOST_CHECK(t1.lookup(0) == false);
        t1[0] = 0;
	BOOST_CHECK(t1.lookup(0));
	BOOST_CHECK(t1[0] == 0 );

}




BOOST_AUTO_TEST_CASE(Massive_CROSS_LOOKUP_TEST){
    std::cout<<"Massive_CROSS_LOOKUP_TEST"<<std::endl;
    for(int k = 1; k < 100; ++k)
	{
		table<int,int,5,3,prime_stream> t1;
		for(int i = 0; i < k; ++i){
            t1[i] = i;
			for(int j = 0; j <= i; ++j){
				BOOST_CHECK_MESSAGE(t1.lookup(j), "MiSSING KEY j:"
                                                  + std::to_string(j)
                                                  + " AFTER INSERT " + std::to_string(i)
                                                  + " In Loop " + std::to_string(k));
			}
		}
	}

}


BOOST_AUTO_TEST_CASE(REHASH_TEST){
    std::cout<<"REHASH_TEST"<<std::endl;
    table<int,int,3,2,prime_stream> t1;
        t1[0] = 0;
        t1[1] = 1;
        t1[2] = 2;
        t1[3] = 3;
        t1[4] = 4;
        t1[5] = 5;
	BOOST_CHECK(t1.lookup(0));
	BOOST_CHECK(t1.lookup(1));
	BOOST_CHECK(t1.lookup(2));
	BOOST_CHECK(t1.lookup(3));
	BOOST_CHECK(t1.lookup(4));
	BOOST_CHECK(t1.lookup(5));
}

BOOST_AUTO_TEST_CASE(MassiveLockupTest){
    std::cout<<"MassiveLockupTest"<<std::endl;
    table<int,int> t1;
	for(int i  = 0; i < 1200000; ++i){
        t1[i] = i;
		BOOST_CHECK(t1.lookup(0));
	}
   	for(int i  = 0; i < 1200000; ++i){
		BOOST_CHECK(t1.lookup(i));
	}
	BOOST_CHECK(t1.lookup(0));
	
}

BOOST_AUTO_TEST_CASE(MassiveInsertTest){
    std::cout<<"MassiveInsertTest"<<std::endl;
    table<int,int> t1;

   	for(int i  = 0; i < 1200000; ++i){
        t1[i] = i;
		BOOST_CHECK(t1.lookup(0));
	}
   	for(int i  = 0; i < 1200000; ++i){
		BOOST_CHECK(t1.lookup(i));
	}
	BOOST_CHECK(t1.lookup(0));
	
}

BOOST_AUTO_TEST_CASE(ComplexItemInsertTest){
    std::cout<<"ComplexItemInsertTest"<<std::endl;
    table<std::string,std::string> t1;
    for(unsigned int i = 0; i <12000; ++i){
        t1.insert(std::to_string(i), std::move(std::string("TEST") + std::to_string(i) ));
        int v = static_cast<int>((static_cast<double>(i)/12000) *100);
        if(  v % 5 == 0) {
            std::cout << "Inserted: " << v<<"%"<<std::endl;
        }
    }
    for( int i = 0; i <12000; ++i){
        BOOST_CHECK(t1.lookup(std::to_string(i)));
        BOOST_CHECK(t1[std::to_string(i)] == std::string("TEST") + std::to_string(i) );
    }
}


BOOST_AUTO_TEST_CASE(MoveOperatorTest){
    std::cout<<"MoveOperatorTest"<<std::endl;
    table<int,int> t1;
	for(int i  = 0; i < 1200000; ++i){
        t1[i] = i;
		BOOST_CHECK(t1.lookup(0));
	}
	table<int,int> moved_table = std::move(t1);
	for(int i  = 0; i < 1200000; ++i){
		BOOST_CHECK(moved_table.lookup(i));
	}
	BOOST_CHECK(moved_table.lookup(0));

}


BOOST_AUTO_TEST_CASE(ZERO_FAILURE_TEST){
    std::cout<<"ZERO_FAILURE_TEST"<<std::endl;
    table<int,int> t1;
	int a = 2022;
        t1[0] = 0;

        t1[a] = a;
	BOOST_CHECK(t1.lookup(a));
	BOOST_CHECK(t1.lookup(0));
}
BOOST_AUTO_TEST_CASE(FilterTestCase){
    std::cout<<"FilterTestCase"<<std::endl;
    table<int,int> t1;
   	for(int i  = 0; i < 1200000; ++i){
        t1[i]= i;
		if(t1.lookup(0) == false){
			BOOST_CHECK(false);
		}
    }
	std::list<int> filtered_list = t1.filter([&](const int & item){ return item <= 100; } );
	int i = 0;
	filtered_list.sort();
	for(auto item : filtered_list){
		BOOST_CHECK(item == i);
		i = i+1;
	}

}

BOOST_AUTO_TEST_CASE(RVALUE_REFECRENCE_OPERATOR_GET_SET_TEST){
    std::cout<<"RVALUE_REFECRENCE_OPERATOR_GET_SET_TEST"<<std::endl;
	measure<10000,std::chrono::microseconds> m;

	m.method_to_compare("RVALUE",[](){
		std::string x ="hallo_mein_name_ist_michael_burzan_und_wir_leben_in_einem";
		table<std::string,int> t1;
   		t1[std::move(x)] = 1;
	});
	m.method_to_compare("REF",[](){
		std::string x ="hallo_mein_name_ist_michael_burzan_und_wir_leben_in_einem";
		table<std::string,int> t1;
   		t1[x] = 1;
	});
	std::cout<<m<<"\n";

	
}

BOOST_AUTO_TEST_CASE(SPEED_UP_INSERT_TEST){
    std::cout<<"SPEED_UP_INSERT_TEST"<<std::endl;
	measure<10000,std::chrono::microseconds> m;

	m.method_to_compare("RVALUE_INSERT",[](){
		std::string x ="hallo_mein_name_ist_michael_burzan_und_wir_leben_in_einem";
		std::string y ="hallo_mein_name_ist_michael_burzan_und_wir_leben_in_einem";
		table<std::string,std::string> t1;
   		t1.insert(x,std::move(y));
	});
	m.method_to_compare("REF_INSERT",[](){
		std::string x ="hallo_mein_name_ist_michael_burzan_und_wir_leben_in_einem";
		std::string y ="hallo_mein_name_ist_michael_burzan_und_wir_leben_in_einem";
		table<std::string,std::string> t1;
   		t1[x] = y;
	});
	std::cout<<m<<"\n";

	
}

BOOST_AUTO_TEST_CASE(TABLE_FOR_EACH_EMPTY_TEST){
    table<std::string,std::string> t1;
    int k = 0;
    for(auto [key, value] : t1) {
        UNUSED(key);
        UNUSED(value);
        k++;
    }
    BOOST_CHECK(k == 0);

}


BOOST_AUTO_TEST_CASE(TABLE_FOR_EACH_TEST){
    table<std::string,std::string> t1;
    t1["A"] = "HALLO";
    t1["B"] = "WELT";
    t1["C"] = "C++";
    int k = 0;
    for(auto [key, value]   : t1){
        UNUSED(key);
        UNUSED(value);
        k++;
    }
    BOOST_CHECK(k == 3);
}

BOOST_AUTO_TEST_CASE(TABLE_FOR_EACH_OVERWRITE_TEST){
    table<std::string,std::string> t1;
    t1["A"] = "HALLO";
    t1["B"] = "WELT";
    t1["C"] = "C++";
    for(auto [key, value]   : t1){
       value = "WELT";
    }

    BOOST_CHECK(t1["A"] == "WELT");
    BOOST_CHECK(t1["B"] == "WELT");
    BOOST_CHECK(t1["C"] == "WELT");
}


BOOST_AUTO_TEST_SUITE_END()
