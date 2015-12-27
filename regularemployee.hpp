// (C) 2013-2015, Sergei Zaychenko, KNURE, Kharkiv, Ukraine

#ifndef _REGULAREMPLOYEE_HPP_
#define _REGULAREMPLOYEE_HPP_

/*****************************************************************************/

#include "employee.hpp"
#include "regularemployeelevel.hpp"
#include <string>

/*****************************************************************************/


class RegularEmployee
	:	public Employee
{

public:
	RegularEmployee(std::string const & _employeeName,
		RegularEmployeeLevel _lvl = RegularEmployeeLevel::Junior, double _salaryBonus = 0 );

	RegularEmployeeLevel const getEmployeeLevel() const { return m_employeeLevel; }
	void levelUp();

	virtual double calculateSalaryPerMounth(double _baseSalary);
	virtual double calculateSalaryPerYear(double _baseSalary);

private:
	RegularEmployeeLevel m_employeeLevel;


};

/*****************************************************************************/

#endif // _REGULAREMPLOYEE_HPP_
