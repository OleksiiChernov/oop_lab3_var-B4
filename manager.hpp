// (C) 2013-2015, Sergei Zaychenko, KNURE, Kharkiv, Ukraine

#ifndef _MANAGER_HPP_
#define _MANAGER_HPP_

/*****************************************************************************/

#include "employee.hpp"
#include <vector>
#include <string>

/*****************************************************************************/


class Manager
	:	public Employee
{
public:

	Manager(std::string _name)
		: Employee(_name)
	{}

	std::vector <Employee *> const & getSubordinateEmployee() const { return m_subordinateEmployee; }

	float getManagerSalary(float _baseSalary);
	void setNewEmployee(std::string const & _fullName);
	void deleteEmployee(std::string const & _fullName);
	bool hasSubordinatedEmployee(std::string const & _fullName);
	virtual float calculateSalaryPerMounth(float _baseSalary);
	void fireManager();

private:

	std::vector < Employee * > m_subordinateEmployee;


};

/*****************************************************************************/


#endif // _MANAGER_HPP_
