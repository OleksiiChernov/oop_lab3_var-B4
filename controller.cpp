// (C) 2013-2015, Sergei Zaychenko, KNURE, Kharkiv, Ukraine

#include "controller.hpp"
#include "messages.hpp"

#include <stdexcept>
#include <algorithm>
#include <string>


int Controller::getRegularEmployeeNumberAtContainer(std::string const & _fullName) const
{
	auto itr = std::find(m_regularEmployee.begin(), m_regularEmployee.end(), _fullName);
	if (itr != m_regularEmployee.end())
	{
		int a = (int)*itr;
		return a;
	}
	else
		return -1;
}

int Controller::getManagerNumberAtContainer(std::string const & _fullName) const
{
	auto itr = std::find(m_managers.begin(), m_managers.end(), _fullName);
	if (itr != m_managers.end())
	{
		int a = (int)*itr;
		return a;
	}
	else
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
	else
		throw std::logic_error(Messages::UnregisteredEmployeeName);
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
	if(a != -1)
		m_regularEmployee[a]->setSalaryBonus(_newBonus);
	else
		throw std::logic_error(Messages::UnregisteredEmployeeName);
}


double Controller::getEmployeeBonus(std::string const & _fullName) const
{
	int a = getRegularEmployeeNumberAtContainer(_fullName);
	if (a != -1)
		m_regularEmployee[a]->getSalaryBonus();
	else
		throw std::logic_error(Messages::UnregisteredEmployeeName);
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


void Controller::assignManager(std::string const & _employeeFullName, std::string const & _managerFullName)
{
	int a = getRegularEmployeeNumberAtContainer(_managerFullName);
	if (a > 0)
		throw std::logic_error(Messages::NotAManager);

	auto it = std::find(m_managers.begin(), m_managers.end(), _managerFullName);
	if (it != m_managers.end())
	{
		int a = (int)*it;
		m_managers[a]->setNewEmployee(_employeeFullName);

		int b = getRegularEmployeeNumberAtContainer(_employeeFullName);
		if(b > 0)
			m_regularEmployee[b]->setManagerName(_managerFullName);
		else
		{
			int c = getManagerNumberAtContainer(_employeeFullName);
			if(c > 0)
				m_managers[c]->setManagerName(_managerFullName);
		}
	}
	else
		throw std::logic_error(Messages::UnregisteredEmployeeName);

}


void Controller::deassignManager(std::string const & _employeeFullName)
{
	auto it = std::find(m_regularEmployee.begin(), m_regularEmployee.end(), _employeeFullName);
	auto itr = std::find(m_managers.begin(), m_managers.end(), _employeeFullName);

	if (it != m_regularEmployee.end())
	{
		int a = (int)*it;
		if (m_regularEmployee[a]->getManagerName() != "")
		{
			for (Manager * _mng : m_managers)
			{
				if (_mng->hasSubordinatedEmployee(_employeeFullName))
				{
					_mng->deleteEmployee(_employeeFullName);
					if (it != m_regularEmployee.end())
					{
						int a = (int)*it;
						m_regularEmployee[a]->delManager();
					}
					else
					{
						if (itr != m_managers.end())
						{
							int b = (int)*itr;
							m_managers[b]->delManager();
						}
					}
				}
			} // for Manager
		}
		else
			throw std::logic_error(Messages::EmployeeHasNoManager);
	}
	else if (itr != m_managers.end())
	{
		int a = (int)*itr;
		if (m_managers[a]->getManagerName() != "")
		{
			for (Manager * _mng : m_managers)
			{
				if (_mng->hasSubordinatedEmployee(_employeeFullName))
				{
					_mng->deleteEmployee(_employeeFullName);
					if (it != m_regularEmployee.end())
					{
						int a = (int)*it;
						m_regularEmployee[a]->delManager();
					}
					else
					{
						if (itr != m_managers.end())
						{
							int b = (int)*itr;
							m_managers[b]->delManager();
						}
					}
				}
			} // for Manager
		}
		else
			throw std::logic_error(Messages::EmployeeHasNoManager);
	}
	else
		throw std::logic_error(Messages::UnregisteredEmployeeName);
		
} 


void Controller::promote(std::string const & _employeeFullName)
{
	int a = getRegularEmployeeNumberAtContainer(_employeeFullName);
	int b = getManagerNumberAtContainer(_employeeFullName);
	if (a > 0)
	{
		m_regularEmployee[a]->levelUp();
	}
	else if (b > 0)
		throw std::logic_error(Messages::CannotPromoteManagers);
	else
		throw std::logic_error(Messages::UnregisteredEmployeeName);
}


void Controller::fire(std::string const & _employeeFullName)
{
	int a = getManagerNumberAtContainer(_employeeFullName);
	if (a > 0)
	{
		m_managers[a]->fireManager();

		if (m_managers[a]->getSubordinateEmployee().size() > 0)
			throw std::logic_error(Messages::CannotFireAssignedManager);
	}
	else
		throw std::logic_error(Messages::UnregisteredEmployeeName);
}


