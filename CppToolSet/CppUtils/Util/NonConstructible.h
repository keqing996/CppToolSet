#pragma once

class NonConstructible
{
public:
    NonConstructible() = delete;
    ~NonConstructible() = delete;
};