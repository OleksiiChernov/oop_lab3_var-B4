// (C) 2013-2015, Sergei Zaychenko, KNURE, Kharkiv, Ukraine

#ifndef _EMPLOYEE_HPP_
#define _EMPLOYEE_HPP_
#include <string>

/*****************************************************************************/


class Employee
{
public:
	
	virtual  ~Employee() = default;
	Employee(std::string const & _employeeName, double _salaryBonus = 0);

	Employee(const Employee &) = delete;
	Employee & operator = (const Employee &) = delete;

	std::string const & getEmployeeName() const { return m_employeeName; }
	double const getSalaryBonus() const { return m_salaryBonus; }
	void setSalaryBonus(double _bonus);
	std::string const & getManagerName()const { return m_managerName; }
	void setManagerName(std::string  _managerName);
	void delManager();

	virtual double calculateSalaryPerMounth(double _baseSalary);
	virtual double calculateSalaryPerYear(double _baseSalary);

	

private:
	std::string m_employeeName;
	double m_salaryBonus;
	std::string m_managerName;
};


/*****************************************************************************/


#endif // _EMPLOYEE_HPP_
