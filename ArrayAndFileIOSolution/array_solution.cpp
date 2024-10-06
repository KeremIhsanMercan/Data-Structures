/* @Author
Student Name : Kerem Ýhsan Mercan
Student ID : 150210042
Date : 16.10.2023 */

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <time.h>
using namespace std;

class Employee {
    private:
        int id;
        int salary;
        int department;
    public:
        void set_id(int new_id) {
            id = new_id;
        }
        void set_salary(int new_salary) {
            salary = new_salary;
        }
        void set_department(int new_department) {
            department = new_department;
        }
        void construct_employee(int x, int y, int z) {
            set_id(x);
            set_salary(y);
            set_department(z);
        }
        int get_id() {
            return id;
        }
        int get_salary() {
            return salary;
        }
        int get_department() {
            return department;
        }
        
};

bool is_data(string string);
int is_valid(int id_to_check, Employee *employee_arr, int array_length);

int main(int argc, char **argv) {

    // clock_t start = clock();

    if (argc != 3) {
        cout << argc << " file provided! It is not the right amount." << endl;
        return 0;
    }

    int allocated_array_length = 500000; // for each line in the operation.txt file
    Employee *employee_array = new Employee[allocated_array_length];

    fstream hw1(argv[1]);

    string one_line;
    string id_temp, salary_temp, department_temp;
    
    int employee_array_length = 0;

    while (getline(hw1, one_line, '\n')) {

        if (!is_data(one_line))
        {
            continue;
        }
        
        stringstream one_line_split(one_line);

        getline(one_line_split, id_temp, ';');
        getline(one_line_split, salary_temp, ';');
        getline(one_line_split, department_temp, '\n');

        employee_array[employee_array_length].construct_employee(stoi(id_temp), stoi(salary_temp), stoi(department_temp));

        employee_array_length++;
    }

    hw1.close();



    fstream operations_file_var(argv[2]);

    string **operation_name_2d_array;
    operation_name_2d_array = new string*[allocated_array_length]; // for line of operations

    for (int i = 0; i < allocated_array_length; i++)
    {
        operation_name_2d_array[i] = new string[4]; // for one line
    }
    

    string operation_name;
    int index_x = 0;

    while (getline(operations_file_var, one_line, '\n')) { // storing operations in a 2d string array

        stringstream one_line_split(one_line);

        getline(one_line_split, operation_name, ';');

        if (operation_name == "ADD")
        {
            string new_emp_salary, new_emp_department;
            getline(one_line_split, new_emp_salary, ';');
            getline(one_line_split, new_emp_department, '\n');

            operation_name_2d_array[index_x][0] = operation_name;
            operation_name_2d_array[index_x][1] = new_emp_salary;
            operation_name_2d_array[index_x][2] = new_emp_department; // 3 times because ADD has 3 entries

        } else if (operation_name == "UPDATE")
        {
            string id_to_update, updated_salary, updated_department;
            getline(one_line_split, id_to_update, ';');
            getline(one_line_split, updated_salary, ';');
            getline(one_line_split, updated_department, '\n');

            operation_name_2d_array[index_x][0] = operation_name;
            operation_name_2d_array[index_x][1] = id_to_update;
            operation_name_2d_array[index_x][2] = updated_salary;
            operation_name_2d_array[index_x][3] = updated_department; // 4 times because UPDATE has 3 entries

        } else if (operation_name == "DELETE")
        {
            string id_to_delete;
            getline(one_line_split, id_to_delete, '\n');
        
            operation_name_2d_array[index_x][0] = operation_name;
            operation_name_2d_array[index_x][1] = id_to_delete; // 2 times because DELETE has 3 entries

        }

        index_x++;
    }


    Employee *final_emp_arr = new Employee[employee_array_length+index_x];

    for (int i = 0; i < employee_array_length; i++) {
        final_emp_arr[i] = employee_array[i];
    }

    delete[] employee_array;
    
    
    string add_str = "ADD";
    string update_str = "UPDATE";
    string delete_str = "DELETE";
    int highest_id = final_emp_arr[employee_array_length-1].get_id();

    for (int i = 0; i < index_x; i++)
    {

        if (operation_name_2d_array[i][0].compare(add_str) == 0)
        {
            // clock_t start = clock();
            
            int new_emp_salary = stoi(operation_name_2d_array[i][1]);
            int new_emp_department = stoi(operation_name_2d_array[i][2]);
            final_emp_arr[employee_array_length].construct_employee(highest_id+1, new_emp_salary, new_emp_department);

            highest_id++;
            employee_array_length++;

            // clock_t end = clock();
            // cout << "ADD: array solution " << (double)(end-start)*1000 / CLOCKS_PER_SEC << " miliseconds" << endl;

        } else if (operation_name_2d_array[i][0] == update_str)
        {
            // clock_t start = clock();

            int id_to_update = stoi(operation_name_2d_array[i][1]);
            int emp_new_salary = stoi(operation_name_2d_array[i][2]);
            int emp_new_department = stoi(operation_name_2d_array[i][3]);

            int arr_element = is_valid(id_to_update, final_emp_arr, employee_array_length);

            if (arr_element != -1) {

                final_emp_arr[arr_element].set_salary(emp_new_salary);
                final_emp_arr[arr_element].set_department(emp_new_department);

            } else {
                cout << "ERROR: An invalid ID to update" << endl;
                continue;
            }

            // clock_t end = clock();
            // cout << "UPDATE: array solution " << (double)(end-start)*1000 / CLOCKS_PER_SEC << " miliseconds" << endl;

        } else if (operation_name_2d_array[i][0] == delete_str)
        {
            // clock_t start = clock();

            int id_to_delete = stoi(operation_name_2d_array[i][1]);

            if (employee_array_length <= 0)
            {
                cout << "ERROR: There is no Employee" << endl;
                continue;
            }

            int arr_element = is_valid(id_to_delete, final_emp_arr, employee_array_length);

            if (arr_element != -1)
            {

                for (int j = 0; j < employee_array_length; j++)
                {
                    if (j <= arr_element) {
                        continue;
                    } else {
                        final_emp_arr[j-1] = final_emp_arr[j];
                    }
                }
                employee_array_length--;
                
            } else {
                cout << "ERROR: An invalid ID to delete" << endl;
                continue;
            }

            // clock_t end = clock();
            // cout << "DELETE: array solution " << (double)(end-start)*1000 / CLOCKS_PER_SEC << " miliseconds" << endl;

        }
    }
    

    for (int i = 0; i < allocated_array_length; i++) {
        delete[] operation_name_2d_array[i]; // for deleting each line
    }
    
    delete[] operation_name_2d_array; // after deleting lines, deleting whole array

    ofstream solution_file("array_solution.csv");

    for (int i = 0; i < employee_array_length; i++)
    {
        solution_file << final_emp_arr[i].get_id() << ";" << final_emp_arr[i].get_salary() << ";" << final_emp_arr[i].get_department() << endl;
    }

    solution_file.close();
    delete[] final_emp_arr;

    // clock_t end = clock();
    // cout << "execution time of all array solution program: " << (double)(end-start)*1000 / CLOCKS_PER_SEC << " miliseconds!" << endl;
    
    return 0;
}

int is_valid(int id_to_check, Employee *employee_arr, int array_length) { // returns the order of the element

    for (int i = 0; i < array_length; i++)
    {
        if (employee_arr[i].get_id() == id_to_check)
        {
            return i;        
        }
    }
    
    return -1; // returns -1 if non valid
}

bool is_data(string string) { // to skip first line in the file

    for (int i = 0; i < (int)string.length()-1; i++)
    {
        if (!isdigit(string[i]))
        {
            if (!(string[i] == ';'))
            {
                return false;
            }
        }
    }

    return true;
}
