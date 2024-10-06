/* @Author
StudentName : Kerem Ýhsan Mercan
StudentID : 150210042
Date : 07.01.2024 */

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
        Employee(int i, int s, int d)
        {
            id = i;
            salary = s;
            department = d;
        }
        void set_id(int new_id)
        {
            id = new_id;
        }
        void set_salary(int new_salary)
        {
            salary = new_salary;
        }
        void set_department(int new_department)
        {
            department = new_department;
        }
        int get_id()
        {
            return id;
        }
        int get_salary()
        {
            return salary;
        }
        int get_department()
        {
            return department;
        }
};

class DoublySkipList_Node {
    private:
        Employee* data;
        DoublySkipList_Node* next;
        DoublySkipList_Node* below;
    public:
        DoublySkipList_Node(Employee *data)
        {
            this->data = data;
            this->next = NULL;
            this->below = NULL;
        }
        void set_emp(Employee* data)
        {
            this->data = data;
        }
        void set_next(DoublySkipList_Node* input)
        {
            this->next = input;
        }
        void set_below(DoublySkipList_Node* input)
        {
            this->below = input;
        }
        Employee* get_emp()
        {
            return this->data;
        }
        DoublySkipList_Node* get_next()
        {
            return this->next;
        }
        DoublySkipList_Node* get_below()
        {
            return this->below;
        }
        int get_next_id()
        {
            if (this->get_next() == NULL)
            {
                return INT32_MAX;
            }
            
            return this->get_next()->get_emp()->get_id();
        }
};

class DoublySkipList {
    private:
        DoublySkipList_Node* head;
        int height;
    public:
        DoublySkipList(int height_in)
        {
            height = height_in;

            Employee* head_emp = new Employee(-1, 0, 0);
            head = new DoublySkipList_Node(head_emp);
            head->set_next(NULL);
            head->set_below(NULL);

            DoublySkipList_Node* iter = head;
            for (int i = 0; i < height_in-1; i++)
            {
                iter->set_below(new DoublySkipList_Node(head_emp));
                iter = iter->get_below();
            }
            
        }
        ~DoublySkipList()
        {
            DoublySkipList_Node* current = head;
            
            while (current != NULL)
            {
                DoublySkipList_Node* level_iter = current;
                current = current->get_below();

                while (level_iter != NULL)
                {
                    DoublySkipList_Node* level_iter_prev = level_iter;
                    level_iter = level_iter->get_next();
                    if (level_iter_prev->get_below() == NULL)
                    {
                        delete level_iter_prev->get_emp();
                        // cout <<"emp deleted\n";
                    }
                    delete level_iter_prev;
                    // cout <<"node deleted\n";
                    
                }
                
            }
            
        }
        void insert(Employee* data)
        {
            int node_height_count = 0;

            while (node_height_count < (height-1) && (rand() % 2) == 1)
            {
                node_height_count++;
            }

            DoublySkipList_Node* current = head;

            int rows_to_skip = height - (node_height_count+1);

            while (rows_to_skip > 0)
            {
                if (current->get_next_id() > data->get_id())
                {
                    current = current->get_below();
                    rows_to_skip--;
                } else
                {
                    current = current->get_next();
                }
            }
            
            while (current->get_next_id() < data->get_id())
            {
                current = current->get_next();
            }
            DoublySkipList_Node* new_node = new DoublySkipList_Node(data);
            new_node->set_next(current->get_next());
            current->set_next(new_node);

            for (int i = 0; i < (node_height_count); i++)
            {
                current = current->get_below();
                while (current->get_next_id() < data->get_id())
                {
                    current = current->get_next();
                }
                new_node->set_below(new DoublySkipList_Node(data));
                new_node = new_node->get_below();
                new_node->set_next(current->get_next());
                current->set_next(new_node);
            }

        }
        void remove(int remove_id)
        {

            DoublySkipList_Node* bottom_left = head;

            while (bottom_left->get_below() != NULL)
            {
                bottom_left = bottom_left->get_below();
            }
            
            if (bottom_left->get_next() == NULL)
            {
                cout << "ERROR: There is no Employee" << "\n";
                return;
            }

            DoublySkipList_Node* current = head;
            DoublySkipList_Node* current_p;
            
            while (current->get_emp()->get_id() != remove_id)
            {
                current_p = current;
                if (current->get_next_id() > remove_id)
                {
                    current = current->get_below();
                } else
                {
                    current = current->get_next();
                }

                if (current == NULL)
                {
                    cout << "ERROR: An invalid ID to delete" << "\n";
                    return;
                }
            }


            while (current_p != NULL)
            {
                while (current_p->get_next_id() != remove_id)
                {
                    current_p = current_p->get_next();
                }

                if (current_p->get_below() == NULL)
                {
                    delete current->get_emp();
                }
                
                current_p->set_next(current->get_next());
                
                DoublySkipList_Node* current_above = current;
                current = current->get_below();
                delete current_above;

                current_p = current_p->get_below();

            }
            
        }
        Employee* search(int search_id)
        {
            DoublySkipList_Node* current = head;
            while (current->get_emp()->get_id() != search_id)
            {
                if (current->get_next_id() > search_id)
                {
                    current = current->get_below();
                } else
                {
                    current = current->get_next();
                }

                if (current == NULL)
                {
                    return NULL;
                }
            }

            return current->get_emp();
        }
        void update_emp(int id_to_update, int new_salary, int new_department)
        {

            Employee* emp_to_update = search(id_to_update);

            if (emp_to_update == NULL)
            {
                cout << "ERROR: An invalid ID to update" << "\n";
                return;
            }
            
            emp_to_update->set_salary(new_salary);
            emp_to_update->set_department(new_department);

        }
        void dumpToFile(ofstream& out_file)
        {
            DoublySkipList_Node* iter = head;

            while (iter->get_below() != NULL)
            {
                iter = iter->get_below();
            }

            iter = iter->get_next(); // to skip head node (-1)

            while (iter != NULL)
            {
                Employee* iter_emp = iter->get_emp();
                out_file << iter_emp->get_id() << ";" << iter_emp->get_salary() << ";" << iter_emp->get_department() << "\n";
                iter = iter->get_next();
            }
        }
        void print_all_levels_front_to_back()
        {
            DoublySkipList_Node* iter_row = head;
            DoublySkipList_Node* iter_col = head;
            while (iter_col != NULL)
            {
                while (iter_row != NULL)
                {
                    cout<< iter_row->get_emp()->get_id()<< " ";
                    iter_row = iter_row->get_next();
                }
                cout << "\n";
                iter_col = iter_col->get_below();
                iter_row = iter_col;
            }
        }
};

int main(int argc, char** argv) {
    
    // clock_t start = clock();

    if (argc != 3) {
        cout << argc << " file provided! It is not the right amount." << "\n";
        return 0;
    }

    srand(time(0));

    DoublySkipList my_skip_list(20);
    
    fstream data_set(argv[1]);

    string one_line;
    string id_temp, salary_temp, department_temp;
    string first_line;
    bool is_first_line_skipped = false;
    int highest_id;

    while (getline(data_set, one_line, '\n')) {
        if (!is_first_line_skipped) {
            first_line = one_line;
            is_first_line_skipped = true;
            continue;
        }
        
        stringstream one_line_split(one_line);

        getline(one_line_split, id_temp, ';');
        getline(one_line_split, salary_temp, ';');
        getline(one_line_split, department_temp, '\n');

        Employee* new_employee = new Employee(stoi(id_temp), stoi(salary_temp), stoi(department_temp));

        my_skip_list.insert(new_employee);

        highest_id = max(highest_id, stoi(id_temp));

    }
    data_set.close();

    fstream operations_file_var(argv[2]);
    string operation_name;

    while (getline(operations_file_var, one_line, '\n')) {

        stringstream one_line_split(one_line);
        getline(one_line_split, operation_name, ';');        

        if (operation_name == "ADD") {
            string new_emp_salary, new_emp_department;
            getline(one_line_split, new_emp_salary, ';');
            getline(one_line_split, new_emp_department, '\n');

            highest_id++;

            Employee* new_employee = new Employee(highest_id, stoi(new_emp_salary), stoi(new_emp_department));
        
            my_skip_list.insert(new_employee);

        } else if (operation_name == "UPDATE") {
            string id_to_update, updated_salary, updated_department;
            getline(one_line_split, id_to_update, ';');
            getline(one_line_split, updated_salary, ';');
            getline(one_line_split, updated_department, '\n');

            my_skip_list.update_emp(stoi(id_to_update), stoi(updated_salary), stoi(updated_department));

        } else if (operation_name == "DELETE") {
            string id_to_delete;
            getline(one_line_split, id_to_delete, '\n');

            my_skip_list.remove(stoi(id_to_delete));

        }
    }
    operations_file_var.close();

    ofstream output_file_var("doubly_output.csv");
    output_file_var << first_line << "\n";
    my_skip_list.dumpToFile(output_file_var);

    output_file_var.close();

    // clock_t end = clock();
    // cout << (double)(end - start) * 1000 / CLOCKS_PER_SEC << "\n";

    return 0;
}