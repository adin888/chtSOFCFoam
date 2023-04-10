#ifndef electrochemistryConstants_H
#define electrochemistryConstants_H

#include "dimensionedScalar.H"

namespace Foam
{
    namespace eletrochemistryConstant
    {
        const dimensionedScalar F
        (
            "F",
            dimensionSet(0, 0, 1, 0, -1, 1, 0),
            96485.3399
        );
        
        const dimensionedScalar FF = F*1e3;
    };
}

#endif
