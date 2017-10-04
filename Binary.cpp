#include "Binary.h"

using namespace std;

BinaryAttribute::BinaryAttribute() {
}

BinaryAttribute::BinaryAttribute(string n) : Attribute::Attribute(n) {
}

void BinaryAttribute::print(ostream& os) const {
	os << "@attribute " << name << " 0 1";
}

ostream& operator<<(ostream& os, const BinaryAttribute &attribute) {
	os << "@attribute " << attribute.name << " 0 1";
	return os;
}