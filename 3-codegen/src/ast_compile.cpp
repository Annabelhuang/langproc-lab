#include "ast.hpp"

#include <string>
#include <regex>

static int makeNameUnq=0;

static std::string makeName(std::string base)
{
    return "_"+base+"_"+std::to_string(makeNameUnq++);
}

void CompileRec(
    std::string destReg,    // The name of the register to put the result in
    TreePtr program
){
    std::regex reNum("^-?[0-9]+$");
    std::regex reId("^[a-z][a-z0-9]*$");
    
    if( regex_match( program->type, reNum ) ){
        std::cout<<"const "<<destReg<<" "<<program->type<<std::endl;
        
    }else if( regex_match( program->type, reId ) ){
        std::cout<<"add "<<destReg<<" "<<program->type<<" "<<" zero "<<std::endl;

    }else if(program->type=="Param"){
        std::cout<<"param "<<destReg<<" "<<program->value<<std::endl;

    }else if(program->type=="Input"){
        std::cout<<"input "<<destReg<<std::endl;

    }else if(program->type=="Output"){
        CompileRec(destReg, program->branches[0]);
        std::cout<<"output "<<destReg<<std::endl;

    }else if(program->type=="LessThan"){
        std::string srcRegA = makeName("srcRegA");
        CompileRec(srcRegA, program->branches[0]);
        std::string srcRegB = makeName("srcRegB");
        CompileRec(srcRegB, program->branches[1]);
        std::cout<<"lt "<<destReg<<" "<<srcRegA<<" "<<srcRegB<<std::endl;

    }else if(program->type=="Add"){
        std::string srcRegA = makeName("asrcRegA");
        CompileRec(srcRegA, program->branches[0]);
        std::string srcRegB = makeName("srcRegB");
        CompileRec(srcRegB, program->branches[1]);
        std::cout<<"add "<<destReg<<" "<<srcRegA<<" "<<srcRegB<<std::endl;

    }else if(program->type=="Sub"){
        std::string srcRegA = makeName("srcRegA");
        CompileRec(srcRegA, program->branches[0]);
        std::string srcRegB = makeName("srcRegB");
        CompileRec(srcRegB, program->branches[1]);
        std::cout<<"sub "<<destReg<<" "<<srcRegA<<" "<<srcRegB<<std::endl;

    }else if(program->type=="Assign"){
        std::string v_reg = makeName("v");
        CompileRec(v_reg, program->branches[0]);
        std::cout<<"add "<<program->value<<" "<<v_reg<<" "<<" zero "<<std::endl;
        std::cout<<"add "<<destReg<<" "<<v_reg<<" "<<" zero "<<std::endl;

    }else if(program->type=="If"){
        std::string srcRegA = makeName("srcRegA");
        CompileRec(srcRegA, program->branches[0]);
        std::string else_label = makeName("else");
        std::string end_label = makeName("endif");
        std::cout<<"beq "<<srcRegA<<" zero "<<else_label<<std::endl;
        CompileRec(destReg, program->branches[1]);
        std::cout<<"beq zero zero "<<end_label<<std::endl;
        std::cout<<":"<<else_label<<std::endl;
        CompileRec(destReg, program->branches[2]);
        std::cout<<":"<<end_label<<std::endl;
    }else if (program->type == "While") {
        std::string loop_start = makeName("loop_start");
        std::string loop_end = makeName("loop_end");
        std::string srcRegA = makeName("srcRegA");
        std::cout << ":" << loop_start << std::endl;
        CompileRec(srcRegA, program->branches[0]);
        std::cout << "beq " << srcRegA << " zero " << loop_end << std::endl;
        CompileRec(destReg, program->branches[1]);
        std::cout << "beq zero zero " << loop_start << std::endl;
        std::cout << ":" << loop_end << std::endl;
        std::cout << "const " << destReg << " 0" << std::endl;
    }
    else if(program->type=="Seq"){
        for(unsigned i=0; i<program->branches.size(); i++){
            CompileRec(destReg, program->branches[i]);
        }
    }else{
        throw std::runtime_error("Unknown construct '"+program->type+"'");
    }
}

void Compile(
    TreePtr program
){
    std::cout << "const zero 0" << std::endl;
    // Create a register to hold the final result
    std::string res=makeName("res");
    
    // Compile the whole thing
    CompileRec(res, program);
    
    // Put the result out
    std::cout<<"halt "<<res<<"\n";
}