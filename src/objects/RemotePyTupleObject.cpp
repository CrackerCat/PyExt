#include "RemotePyTupleObject.h"

#include "objects.h"
#include "utils/lossless_cast.h"

#include <engextcpp.hpp>

#include <memory>
#include <string>
#include <vector>
#include <sstream>
#include <iomanip>
#include <stdexcept>
using namespace std;

RemotePyTupleObject::RemotePyTupleObject(Offset objectAddress)
	: RemotePyVarObject(objectAddress, "PyTupleObject")
{
}


auto RemotePyTupleObject::numItems() const -> SSize
{
	return RemotePyVarObject::size();
}


auto RemotePyTupleObject::at(SSize index) const -> unique_ptr<RemotePyObject>
{
	if (index < 0 || index >= numItems())
		throw out_of_range("RemotePyListObject::at index out of range.");

	auto obj = remoteObj();
	auto itemPtr = obj.Field("ob_item").ArrayElement(index).GetPtr();
	return makeRemotePyObject(itemPtr);
}


auto RemotePyTupleObject::listValue() const -> vector<unique_ptr<RemotePyObject>>
{
	auto count = utils::lossless_cast<size_t>(numItems());
	vector<unique_ptr<RemotePyObject>> values(count);

	for (size_t i = 0; i < count; ++i) {
		values[i] = at(i);
	}

	return values;
}


auto RemotePyTupleObject::repr(bool /*pretty*/) const -> string
{
	ostringstream oss;
	oss << "(";

	auto count = numItems();
	for (SSize i = 0; i < count; ++i) {
		auto elem = at(i);
		if (elem != nullptr)
			oss << elem->repr();

		if (i+1 < count)
			oss << ", ";
	}

	oss << ")";
	return oss.str();
}