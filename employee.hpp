// (C) 2013-2015, Sergei Zaychenko, KNURE, Kharkiv, Ukraine

#ifndef _EMPLOYEE_HPP_
#define _EMPLOYEE_HPP_
#include <string>

/*****************************************************************************/


class Employee
{
public:
	
	virtual  ~Employee() = default;

	Employee(const Employee &) = delete;
	Employee & operator = (const Employee &) = delete;

	std::string const & getEmployeeName() const { return m_employeeName; }
	double const getSalaryBonus() const { return m_salaryBonus; }
	void setSalaryBonus(double _bonus);

	virtual float calculateSalaryPerMounth(float _baseSalary);
	virtual float calculateSalaryPerYear(float _baseSalary);

protected:

	Employee(std::string const & _employeeName, double _salaryBonus = 0);

private:
	std::string m_employeeName;
	double m_salaryBonus;
};


/*****************************************************************************/


#endif // _EMPLOYEE_HPP_
