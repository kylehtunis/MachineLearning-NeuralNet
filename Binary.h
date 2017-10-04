#ifndef BINARY_H
#define BINARY_H

#include <iostream>
#include "Attribute.h"
#include <string>

using namespace std;

class BinaryAttribute : public Attribute {
private:
	friend ostream& operator<<(ostream& os, const BinaryAttribute &attribute);
public:
	virtual void print(ostream& out = cout) const;
	BinaryAttribute();
	BinaryAttribute(string n);
};
#endif // !BINARY_H
