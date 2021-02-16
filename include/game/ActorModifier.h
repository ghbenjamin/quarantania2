#pragma once

class ActorModifer
{
public:
    
    ActorModifer( std::string const& name );
    ActorModifer( std::string const& name, int roundsRemaining );
    virtual ~ActorModifer() = default;
    
    std::string const& getName() const;
    bool hasTimeout() const;
    int getRoundsRemaining() const;

    

private:
    std::string m_name;
    bool m_hasTimeout;
    int m_roundsRemaining;

};