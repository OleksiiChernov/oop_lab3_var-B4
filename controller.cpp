// (C) 2013-2015, Sergei Zaychenko, KNURE, Kharkiv, Ukraine

#include "controller.hpp"
#include "messages.hpp"

#include <stdexcept>
#include <algorithm>
#include <string>


Controller::~Controller()
{
	for (Manager * _mng : m_managers)
		delete _mng;

	for (RegularEmployee * _emp : m_regularEmployee)
		delete _emp;
}

int Controller::getRegularEmployeeNumberAtContainer(std::string const & _fullName) const
{
	auto itr = std::find_if(m_regularEmployee.begin(), m_regularEmployee.end(), 
		[_fullName](RegularEmployee const * _empl)
		{
			if (_empl->getEmployeeName() == _fullName)
				return true;
			else
				return false;
		}
	);

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
	auto itr = std::find_if(m_managers.begin(), m_managers.end(), 
		[_fullName](Manager const * _mng)
	{
		if (_mng->getEmployeeName() == _fullName)
			return true;
		else
			return false;
	}
	);
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

	int a = getManagerNumberAtContainer(_fullName);

	if (a == (int)*m_managers.end())
	{
		Manager * mng = new Manager(_fullName);
		m_managers.push_back(mng);
	}
	else
		throw std::logic_error(Messages::EmployeeFullNameNonUnique);
}


void Controller::createRegular(std::string const & _fullName, RegularEmployeeLevel _startingLevel)
{
	int a = getRegularEmployeeNumberAtContainer(_fullName);

	if (a == (int)*m_regularEmployee.end())
	{
		RegularEmployee * empl = new RegularEmployee(_fullName, _startingLevel);
		m_regularEmployee.push_back(empl);
	}
	else
		throw std::logic_error(Messages::EmployeeFullNameNonUnique);
}


bool Controller::isKnownEmployee(std::string const & _fullName) const
{
	int a = getManagerNumberAtContainer(_fullName);

	if (a != (int)*m_managers.end())
		return true;
	else
	{
		int b = getRegularEmployeeNumberAtContainer(_fullName);
		if (b != (int)*m_regularEmployee.end())
			return true;
	}

	return false;
}


RegularEmployeeLevel Controller::getRegularEmployeeLevel(std::string const & _fullName) const
{
	int a = getManagerNumberAtContainer(_fullName);
	if (a != (int)*m_managers.end())
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
		return m_regularEmployee[a]->getSalaryBonus();
	else
		throw std::logic_error(Messages::UnregisteredEmployeeName);
}


std::string Controller::getEmployeeManager(std::string const & _fullName) const
{
	for (Manager * _mng : m_managers)
	{
		auto it = std::find_if(_mng->getSubordinateEmployee().begin(), _mng->getSubordinateEmployee().end(), 
			[_fullName](Employee const * _empl)
			{
				if (_empl->getEmployeeName() == _fullName)
					return true;
				else
					return false;
			}
		);
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
	
	int b = getManagerNumberAtContainer(_fullName);
	if (b != (int)*m_managers.end())
	{
		if (m_managers[b]->getSubordinateEmployee().size() > 0)
		{
			for (Employee * _empl : m_managers[b]->getSubordinateEmployee())
				_employeeNames.insert(_empl->getEmployeeName());
			return _employeeNames;
		}
	}
	else
		return _employeeNames;
}


void Controller::assignManager(std::string const & _employeeFullName, std::string const & _managerFullName)
{
	int a = getRegularEmployeeNumberAtContainer(_managerFullName);
	if (a > 0)
		throw std::logic_error(Messages::NotAManager);

	int d = getManagerNumberAtContainer(_managerFullName);
	if (d != (int)*m_managers.end())
	{
		m_managers[d]->setNewEmployee(_employeeFullName);

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
	int a = getRegularEmployeeNumberAtContainer(_employeeFullName);
	int b = getManagerNumberAtContainer(_employeeFullName);

	if (a != (int)*m_regularEmployee.end())
	{
		if (m_regularEmployee[a]->getManagerName() != "")
		{
			for (Manager * _mng : m_managers)
			{
				if (_mng->hasSubordinatedEmployee(_employeeFullName))
				{
					_mng->deleteEmployee(_employeeFullName);
					if (a != (int)*m_regularEmployee.end())
					{
						m_regularEmployee[a]->delManager();
					}
					else
					{
						if (b != (int)*m_managers.end())
						{
							m_managers[b]->delManager();
						}
					}
				}
			} // for Manager
		}
		else
			throw std::logic_error(Messages::EmployeeHasNoManager);
	}
	else if (b != (int)*m_managers.end())
	{
		if (m_managers[b]->getManagerName() != "")
		{
			for (Manager * _mng : m_managers)
			{
				if (_mng->hasSubordinatedEmployee(_employeeFullName))
				{
					_mng->deleteEmployee(_employeeFullName);
					if (a != (int)*m_regularEmployee.end())
					{
						m_regularEmployee[a]->delManager();
					}
					else
					{
						if (b != (int)*m_managers.end())
						{
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


