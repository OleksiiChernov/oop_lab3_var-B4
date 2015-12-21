// (C) 2013-2015, Sergei Zaychenko, KNURE, Kharkiv, Ukraine

#include "controller.hpp"
#include "messages.hpp"

#include <stdexcept>
#include <algorithm>


int Controller::getRegularEmployeeNumberAtContainer(std::string const & _fullName) const
{
	auto itr = std::find(m_regularEmployee.begin(), m_regularEmployee.end(), _fullName);
	if (itr != m_regularEmployee.end())
	{
		int a = (int)*itr;
		return a;
	}
	else
		throw std::logic_error(Messages::UnregisteredEmployeeName);

	return -1;
}


void Controller::createManager(std::string const & _fullName)
{

	auto it = std::find(m_managers.begin(), m_managers.end(), _fullName);

	if (it == m_managers.end())
	{
		Manager * mng = new Manager(_fullName);
		m_managers.push_back(mng);
	}
	else
		throw std::logic_error(Messages::EmployeeFullNameNonUnique);
}


void Controller::createRegular(std::string const & _fullName, RegularEmployeeLevel _startingLevel)
{
	auto it = std::find(m_regularEmployee.begin(), m_regularEmployee.end(), _fullName);

	if (it == m_regularEmployee.end())
	{
		RegularEmployee * empl = new RegularEmployee(_fullName, _startingLevel);
		m_regularEmployee.push_back(empl);
	}
	else
		throw std::logic_error(Messages::EmployeeFullNameNonUnique);
}


bool Controller::isKnownEmployee(std::string const & _fullName) const
{
	auto it = std::find(m_managers.begin(), m_managers.end(), _fullName);

	if (it != m_managers.end())
		return true;
	else
	{
		auto t = std::find(m_regularEmployee.begin(), m_regularEmployee.end(), _fullName);
		if (t != m_regularEmployee.end())
			return true;
	}

	return false;
}


RegularEmployeeLevel Controller::getRegularEmployeeLevel(std::string const & _fullName) const
{
	auto it = std::find(m_managers.begin(), m_managers.end(), _fullName);
	if (it != m_managers.end())
		throw std::logic_error(Messages::NotARegular);
	else
	{
		int a = getRegularEmployeeNumberAtContainer(_fullName);
		if (a != -1)
			return m_regularEmployee[a]->getEmployeeLevel();
		else
			throw std::logic_error(Messages::UnregisteredEmployeeName);
	}
}


double Controller::getEmployeeSalary(std::string const & _fullName, double _baseSalary)
{
	int a = getRegularEmployeeNumberAtContainer(_fullName);
	if (a != -1)
		return m_regularEmployee[a]->calculateSalaryPerMounth(_baseSalary);
}


double Controller::getTotalSalary(double _baseSalary) const
{
	double _salary = 0;

	for (RegularEmployee * _emp : m_regularEmployee)
		_salary += _emp->calculateSalaryPerMounth(_baseSalary);

	//for (Manager * _mng : m_managers)
	//	_salary += _mng->calculateSalaryPerMounth(_baseSalary);

	return _salary;
}

void Controller::changeBonus(std::string const & _fullName, double _newBonus)
{
	int a = getRegularEmployeeNumberAtContainer(_fullName);
	m_regularEmployee[a]->setSalaryBonus(_newBonus);
}


double Controller::getEmployeeBonus(std::string const & _fullName) const
{
	int a = getRegularEmployeeNumberAtContainer(_fullName);
	m_regularEmployee[a]->getSalaryBonus();
}


std::string Controller::getEmployeeManager(std::string const & _fullName) const
{
	for (Manager * _mng : m_managers)
	{
		auto it = std::find(_mng->getSubordinateEmployee().begin(), _mng->getSubordinateEmployee().end(), _fullName);
		if (it != _mng->getSubordinateEmployee().end())
			return _mng->getEmployeeName();
	}
	throw std::logic_error(Messages::UnregisteredEmployeeName);
}


std::unordered_set<std::string> Controller::getManagerSubordinates(std::string const & _fullName) const
{
	int a = getRegularEmployeeNumberAtContainer(_fullName);
	if (a > 0)
		throw std::logic_error(Messages::NotAManager);
	std::unordered_set <std::string> _employeeNames;
	
	auto it = std::find(m_managers.begin(), m_managers.end(), _fullName);
	if (it != m_managers.end())
	{
		int a = (int)*it;
		if (m_managers[a]->getSubordinateEmployee().size() > 0)
		{
			for (Employee * _empl : m_managers[a]->getSubordinateEmployee())
				_employeeNames.insert(_empl->getEmployeeName());
			return _employeeNames;
		}
		else
			return _employeeNames;
	}
}


