#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <iostream>
#include <thread>
#include <string>
#include <chrono>

using namespace std;
namespace py = pybind11;
using namespace pybind11::literals; // to bring in the `_a` literal
using std::chrono::high_resolution_clock;
using std::chrono::duration_cast;
using std::chrono::duration;
using std::chrono::milliseconds;

class TimeCalculator {
public:
	TimeCalculator() {};
	py::function py_callback;
	void set_callback(py::function cb) {
		py_callback = cb;
	};

	py::dict run(int nb_iter = 100000000, int payload_length = 64, std::uint8_t default_val = 0xf) {
		std::shared_ptr<std::vector<std::uint8_t>> data_received_vector = std::make_shared<std::vector<std::uint8_t>>();
		for (int i = 0; i < payload_length; i++)
			data_received_vector->push_back(default_val);

		std::cout << "Go " << std::endl;
		auto t1 = high_resolution_clock::now();
		for (int i = 0; i < nb_iter; i++) {
			on_callback(data_received_vector);
			//std::cout << "Iter: " << i + 1 << "/" << nb_iter << std::endl;
		}
		auto t2 = high_resolution_clock::now();

		/* Getting number of milliseconds as an integer. */
		auto ms_int = duration_cast<milliseconds>(t2 - t1);

		/* Getting number of milliseconds as a double. */
		duration<double, std::milli> ms_double = t2 - t1;
		std::cout << "End " << std::endl;
		py::dict d("ms_int"_a = ms_int.count(), "ms_double"_a= ms_double.count());
		return d;
	};

	void on_callback(std::shared_ptr<std::vector<std::uint8_t>> data_received_vector) {
		//py::gil_scoped_release release;  // release the GIL
		py::gil_scoped_acquire acquire;
		std::vector<std::uint8_t> data(data_received_vector->begin(), data_received_vector->end());
		//ToDo: throw the Frame dessicted (create the a dynamic reper of the frame)
		py_callback(data);
	};

};
int add(int i, int j) {
    return i + j;
}


class Pet
{
public:
    Pet(const std::string& name, int hunger) : name(name), hunger(hunger) {}
    ~Pet() {}

    void go_for_a_walk() { hunger++; }
    const std::string& get_name() const { return name; }
    int get_hunger() const { return hunger; }

private:
    std::string name;
    int hunger;
};


namespace py = pybind11;

PYBIND11_MODULE(pyb11demo, m) {
    // optional module docstring
    m.doc() = "pybind11 example plugin";

    // define add function
    m.def("add", &add, "A function which adds two numbers");

    // bindings to Pet class
    py::class_<Pet>(m, "Pet")
        .def(py::init<const std::string&, int>())
        .def("go_for_a_walk", &Pet::go_for_a_walk)
        .def("get_hunger", &Pet::get_hunger)
        .def("get_name", &Pet::get_name);

	py::class_<TimeCalculator> timeCalculator(m, "TimeCalculator");
	timeCalculator.def(py::init<>())
		.def("set_callback", &TimeCalculator::set_callback, "set_callback", py::call_guard<py::gil_scoped_release>())
		.def("run", &TimeCalculator::run, "run"); // , py::call_guard<py::gil_scoped_release>());
}