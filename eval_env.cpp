#include"eval_env.h"
#include"error.h"

ValuePtr EvalEnv::eval(ValuePtr expr){
    if ((*expr).isSelfEvaluating()) {
        return expr;
    }
    else if ((*expr).isNil()) {
        throw LispError("Evaluating nil is prohibited.");
    } 
    else {
        throw LispError("Unimplemented");
    }
}
