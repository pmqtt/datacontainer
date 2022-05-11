//
// Created by cefour on 05.05.22.
//
#define BOOST_TEST_DYN_LINK 1
#define BOOST_TEST_MODULE EmbeddedPythonTest
#include <boost/test/included/unit_test.hpp>
#include <vector>
#include <string>
#include <Python.h>
#include <algorithm>
#include <iostream>
#include <boost/python.hpp>
namespace py = boost::python;





BOOST_AUTO_TEST_SUITE(EmbeddedPythonTestSuite)


    BOOST_AUTO_TEST_CASE(BOOST_EMBEDDED_TEST){
        //In Python "print('Hello World!')Execute
        //py::object global = py::import("__main__").attr("__dict__");
        //ypy::exec("print('Hello World!')", global);

        Py_Initialize(); //Must be called first

        try{


            py::object global = py::import("__main__").attr("__dict__");

            std::string script = "def test(a):\n"
                                 "\treturn a**3\n\n"
                                 "result = test(value)";


            global["value"] = 10;
            py::exec(script.c_str(), global);
            std::cout << py::extract<int>(global["result"]) << std::endl;

        }
        catch (const py::error_already_set &){
            //If an error occurs while executing Python code, the error content is displayed.
            PyErr_Print();
        }

    }

BOOST_AUTO_TEST_SUITE_END()