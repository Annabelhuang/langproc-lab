#include "ast.hpp"

#include <regex>

int32_t Interpret(
    InterpretContext &context, // Contains the parameters and variable bindings
    TreePtr program
){
    std::regex reNum("^-?[0-9]+$");
    std::regex reId("^[a-z][a-z0-9]*$");
    if (regex_match(program->type, reNum)) {
        return std::atol(program->type.c_str());
    } 
    // TODO : Check for things matching reId
    else if (regex_match(program->type, reId)) {
        // Retrieve variable value
        if (context.bindings.find(program->type) == context.bindings.end()) {
            throw std::runtime_error("Undefined variable: " + program->type);
        }
        return context.bindings[program->type];
    }else if(program->type=="Param"){
        unsigned index=atol(program->value.c_str());
        auto value=context.params.at(index);
        return value;
        
    }else if (program->type == "Input") {
        int value;
        std::cin >> value;  
        return value;       
    }else if(program->type=="Output"){
        int32_t val=Interpret(context, program->branches.at(0));
        std::cout<<val<<std::endl;
        return val;
        
    // TODO: Handle other constructs
        
    } else if (program->type == "Add") {
        return Interpret(context, program->branches.at(0)) + 
               Interpret(context, program->branches.at(1));
    }else if (program->type == "Sub") {
        return Interpret(context, program->branches.at(0)) - 
               Interpret(context, program->branches.at(1));
    }else if (program->type == "LessThan") {
        return Interpret(context, program->branches.at(0)) < 
               Interpret(context, program->branches.at(1));
    }else if (program->type == "Assign") {
        int32_t val = Interpret(context, program->branches.at(0));
        context.bindings[program->value] = val;
        return val;
    }else if (program->type == "Seq") {
        int32_t result = 0;
        for (auto &stmt : program->branches) {
            result = Interpret(context, stmt);
        }
        return result;
    }else if (program->type == "If") {
        if (Interpret(context, program->branches.at(0))) {
            return Interpret(context, program->branches.at(1));
        } else {
            return Interpret(context, program->branches.at(2));
        }
    }else if (program->type == "While") {
        while (Interpret(context, program->branches.at(0))) {
            Interpret(context, program->branches.at(1));
        }
        return 0;
    }else{
        throw std::runtime_error("Unknown construct '"+program->type+"'");
    }
}
