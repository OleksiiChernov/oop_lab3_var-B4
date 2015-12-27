// (C) 2013-2015, Sergei Zaychenko, KNURE, Kharkiv, Ukraine

#ifndef _MANAGER_HPP_
#define _MANAGER_HPP_

/*****************************************************************************/

#include "employee.hpp"
#include "regularemployee.hpp"
#include <vector>
#include <string>

/*****************************************************************************/


class Manager
	:	public Employee
{
public:

	Manager(std::string const & _name)
		: Employee(_name)
	{}

	std::vector <Employee *> const & getSubordinateEmployee() const { return m_subordinateEmployee; }

	float getManagerSalary(float _baseSalary);
	void setNewEmployee( Employee & _empl);
	void deleteEmployee(std::string const & _fullName);
	bool hasSubordinatedEmployee(std::string const & _fullName);

private:

	std::vector < Employee * > m_subordinateEmployee;


};

/*****************************************************************************/


#endif // _MANAGER_HPP_
