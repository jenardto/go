/*
 * ParametricValue.h
 *
 *  A wrapper around the expression parser
 *
 *  Created on: Feb 8, 2009
 *      Author: jima
 */

#ifndef PARAMETRICVALUE_H_
#define PARAMETRICVALUE_H_

#include "mathexpr.h"
#include <algebra3.h>

class ParametricValue {
public:
    virtual double getValue() = 0;
    virtual bool good() { return true; }
    virtual ~ParametricValue() {};
};

class ConstValue : public ParametricValue {
    double _value;

public:
    ConstValue(double value) : _value(value) {}

    double getValue() { return _value; }
};

#endif /* PARAMETRICVALUE_H_ */
