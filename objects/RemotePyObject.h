#pragma once

#include <engextcpp.hpp>

#include <cstdint>
#include <string>


// Represents a PyObject in the debuggee's address space. Base class for all types of PyObject.
class RemotePyObject
{

public: // Typdefs.
	using Offset = std::uint64_t;
	using SSize = std::int64_t;

public: // Construction/Destruction.
	explicit RemotePyObject(Offset objectAddress, const std::string& typeName = "PyObject");
	virtual ~RemotePyObject();

public: // Members.
	SSize refCount() const;
	std::string typeName() const;
	virtual std::string repr(bool pretty = true) const;

protected: // Helpers for more derived classes.
	// Access the underlying remote object.
	ExtRemoteTyped remoteObj() const;
	// NOTE: ExtRemoteTyped isn't const-correct.
	// remoteObj() makes a copy so it can be used in const methods.

private: // Data.
	ExtRemoteTyped remoteObj_;

};
