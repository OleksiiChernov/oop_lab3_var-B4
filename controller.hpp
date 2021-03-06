// (C) 2013-2015, Sergei Zaychenko, KNURE, Kharkiv, Ukraine

#ifndef _CONTROLLER_HPP_
#define _CONTROLLER_HPP_

/*****************************************************************************/

#include "regularemployeelevel.hpp"
#include "manager.hpp"
#include "regularemployee.hpp"

#include <string>
#include <unordered_set>
#include <vector>

/*****************************************************************************/


class Controller
{

/*-----------------------------------------------------------------*/

public:

/*-----------------------------------------------------------------*/

	Controller() {};

	Controller ( const Controller & ) = delete;

	Controller & operator = ( const Controller & ) = delete;

	~ Controller ();

/*-----------------------------------------------------------------*/

	void createManager ( std::string const & _fullName );

	void createRegular ( 
			std::string const & _fullName
		,	RegularEmployeeLevel _startingLevel
	);

	bool isKnownEmployee ( std::string const & _fullName ) const;

	RegularEmployeeLevel getRegularEmployeeLevel ( std::string const & _fullName ) const;

	double getEmployeeSalary ( std::string const & _fullName, double _baseSalary );

	double getTotalSalary ( double _baseSalary ) const;

	void changeBonus ( std::string const & _fullName, double _newBonus );

	double getEmployeeBonus ( std::string const & _fullName ) const;

	std::string getEmployeeManager ( std::string const & _fullName ) const;

	std::unordered_set< std::string > getManagerSubordinates ( std::string const & _fullName ) const;

	void assignManager ( std::string const & _employeeFullName, std::string const & _managerFullName );

	void deassignManager ( std::string const & _employeeFullName );

	void promote ( std::string const & _employeeFullName );

	void fire ( std::string const & _employeeFullName );

/*-----------------------------------------------------------------*/
	

private:

	auto getRegularEmployeeNumberAtContainer(std::string const & _fullName) const;
	auto getManagerNumberAtContainer(std::string const & _fullName) const;

	int isEmployeeOrManager(std::string const & _fullname);
	void assignEmployeeToManager(std::string const & _employeeName, std::string const & _managerName);
	void assignManagerToManager(std::string const & _employeeName, std::string const & _managerName);

	std::vector <Manager *> m_managers;
	std::vector <RegularEmployee *> m_regularEmployee;

};


/*****************************************************************************/

#endif //  _CONTROLLER_HPP_
