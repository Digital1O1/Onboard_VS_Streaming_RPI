// g++ main.cpp -o YAML_Example -lyaml-cpp
// WRITE YAML FILE
#include <yaml-cpp/yaml.h>
#include <fstream>
#include <iostream>
#include <string>

int main()
{
    YAML::Emitter out;

    // Start new YAML map
    // Maps in YAML --> Collection of key-value pairs
    // Similar to a dictionary
    out << YAML::BeginMap;

    // Key-value pairs are added here to the map
    out << YAML::Key << "name" << YAML::Value << "John Doe";
    out << YAML::Key << "age" << YAML::Value << 30;
    out << YAML::Key << "married" << YAML::Value << true;

    // Map ends here
    out << YAML::EndMap;

    // Actually writes to the output.yaml file
    std::ofstream fout("output.yaml");
    fout << out.c_str();
    fout.close();

    std::cout << "YAML file created successfully." << std::endl;

    // Reading from YAML file
    try
    {
        // config is just a YAML object
        YAML::Node config = YAML::LoadFile("output.yaml");

        if (config["name"])
        {
            // If node contains key called 'name' extract it as a std::string and print to console
            // .as<T>() method used to convert YAML node to desiered data type, in this case std::string
            std::string name = config["name"].as<std::string>();
            std::cout << "Name: " << name << std::endl;
        }

        if (config["age"])
        {
            int age = config["age"].as<int>();
            std::cout << "Age: " << age << std::endl;
        }

        if (config["married"])
        {
            bool married = config["married"].as<bool>();
            std::cout << "Married: " << (married ? "Yes" : "No") << std::endl;
        }
    }

    // If any exceptions are thrown during reading/parsing of YAML file; like if it's not found, this block catches the exception
    catch (const YAML::Exception &e)
    {
        // e.what() method used to grab the error so it can be printed out
        std::cerr << "Exception while reading YAML file : " << e.what() << std::endl;
    }

    return 0;
}
