#pragma once

class Rational
{
public:

    Rational();
    Rational(int t, int b);
    ~Rational() = default;

    Rational operator+(Rational const& other);

    void reduce();

    int getTop() const;
    void setTop(int top);
    int getBottom() const;
    void setBottom(int bottom);

private:
    int m_top;
    int m_bottom;
};