/*
  Copyright (c) 2009, Hideyuki Tanaka
  All rights reserved.

  Redistribution and use in source and binary forms, with or without
  modification, are permitted provided that the following conditions are met:
  * Redistributions of source code must retain the above copyright
  notice, this list of conditions and the following disclaimer.
  * Redistributions in binary form must reproduce the above copyright
  notice, this list of conditions and the following disclaimer in the
  documentation and/or other materials provided with the distribution.
  * Neither the name of the <organization> nor the
  names of its contributors may be used to endorse or promote products
  derived from this software without specific prior written permission.

  THIS SOFTWARE IS PROVIDED BY <copyright holder> ''AS IS'' AND ANY
  EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
  WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  DISCLAIMED. IN NO EVENT SHALL <copyright holder> BE LIABLE FOR ANY
  DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
  (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
  ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
  SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#pragma once

#include <iostream>
#include <sstream>
#include <vector>
#include <map>
#include <string>
#include <stdexcept>
#include <typeinfo>
#include <cstring>
#include <algorithm>
#include <cstdlib>

#ifdef __GNUC__
#include <cxxabi.h>
#endif

namespace cmdline
{
    namespace detail
    {
        template <typename Target, typename Source, bool Same>
        class lexical_cast_t
        {
        public:
            static Target cast(const Source& arg)
            {
                Target ret;
                std::stringstream ss;
                if (!(ss << arg && ss >> ret && ss.eof()))
                    throw std::bad_cast();

                return ret;
            }
        };

        template <typename Target, typename Source>
        class lexical_cast_t<Target, Source, true>
        {
        public:
            static Target cast(const Source& arg)
            {
                return arg;
            }
        };

        template <typename Source>
        class lexical_cast_t<std::string, Source, false>
        {
        public:
            static std::string cast(const Source& arg)
            {
                std::ostringstream ss;
                ss << arg;
                return ss.str();
            }
        };

        template <typename Target>
        class lexical_cast_t<Target, std::string, false>
        {
        public:
            static Target cast(const std::string& arg)
            {
                Target ret;
                std::istringstream ss(arg);
                if (!(ss >> ret && ss.eof()))
                    throw std::bad_cast();
                return ret;
            }
        };

        template <typename T1, typename T2>
        struct is_same
        {
            static const bool value = false;
        };

        template <typename T>
        struct is_same<T, T>
        {
            static const bool value = true;
        };

        template <typename Target, typename Source>
        Target lexical_cast(const Source& arg)
        {
            return lexical_cast_t<Target, Source, is_same<Target, Source>::value>::cast(arg);
        }

        static inline std::string demangle(const std::string& name)
        {
#ifdef _MSC_VER
            return name;
#elif defined(__GNUC__)
            int status=0;
            char *p=abi::__cxa_demangle(name.c_str(), 0, 0, &status);
            std::string ret(p);
            free(p);
            return ret;
#else
            #error unexpected c complier (msc/gcc), Need to implement this method for demangle
#endif
        }

        template <class T>
        std::string readable_typename()
        {
            return demangle(typeid(T).name());
        }

        template <class T>
        std::string default_value(T def)
        {
            return detail::lexical_cast<std::string>(def);
        }

        template <>
        inline std::string readable_typename<std::string>()
        {
            return "string";
        }
    } // detail

    //-----

    class CmdLineError : public std::exception
    {
    public:
        CmdLineError(const std::string& msg): msg(msg)
        {
        }

        ~CmdLineError() throw() override
        {
        }

        const char* what() const throw() override { return msg.c_str(); }

    private:
        std::string msg;
    };

    template <class T>
    struct DefaultReader
    {
        T operator()(const std::string& str)
        {
            return detail::lexical_cast<T>(str);
        }
    };

    template <class T>
    struct RangeReader
    {
        RangeReader(const T& low, const T& high): _low(low), _high(high)
        {
        }

        T operator()(const std::string& s) const
        {
            T ret = DefaultReader<T>()(s);
            if (!(ret >= _low && ret <= _high)) throw CmdLineError("range_error");
            return ret;
        }

    private:
        T _low;
        T _high;
    };

    template <class T>
    RangeReader<T> Range(const T& low, const T& high)
    {
        return RangeReader<T>(low, high);
    }

    template <class T>
    struct OneofReader
    {
        T operator()(const std::string& s)
        {
            T ret = DefaultReader<T>()(s);
            if (std::find(_alt.begin(), _alt.end(), ret) == _alt.end())
                throw CmdLineError("");
            return ret;
        }

        void Add(const T& v) { _alt.push_back(v); }

    private:
        std::vector<T> _alt;
    };

    template <class T>
    OneofReader<T> OneOf(T a1)
    {
        OneofReader<T> ret;
        ret.Add(a1);
        return ret;
    }

    template <class T>
    OneofReader<T> OneOf(T a1, T a2)
    {
        OneofReader<T> ret;
        ret.Add(a1);
        ret.Add(a2);
        return ret;
    }

    template <class T>
    OneofReader<T> OneOf(T a1, T a2, T a3)
    {
        OneofReader<T> ret;
        ret.Add(a1);
        ret.Add(a2);
        ret.Add(a3);
        return ret;
    }

    template <class T>
    OneofReader<T> OneOf(T a1, T a2, T a3, T a4)
    {
        OneofReader<T> ret;
        ret.Add(a1);
        ret.Add(a2);
        ret.Add(a3);
        ret.Add(a4);
        return ret;
    }

    template <class T>
    OneofReader<T> OneOf(T a1, T a2, T a3, T a4, T a5)
    {
        OneofReader<T> ret;
        ret.Add(a1);
        ret.Add(a2);
        ret.Add(a3);
        ret.Add(a4);
        ret.Add(a5);
        return ret;
    }

    template <class T>
    OneofReader<T> OneOf(T a1, T a2, T a3, T a4, T a5, T a6)
    {
        OneofReader<T> ret;
        ret.Add(a1);
        ret.Add(a2);
        ret.Add(a3);
        ret.Add(a4);
        ret.Add(a5);
        ret.Add(a6);
        return ret;
    }

    template <class T>
    OneofReader<T> OneOf(T a1, T a2, T a3, T a4, T a5, T a6, T a7)
    {
        OneofReader<T> ret;
        ret.Add(a1);
        ret.Add(a2);
        ret.Add(a3);
        ret.Add(a4);
        ret.Add(a5);
        ret.Add(a6);
        ret.Add(a7);
        return ret;
    }

    template <class T>
    OneofReader<T> OneOf(T a1, T a2, T a3, T a4, T a5, T a6, T a7, T a8)
    {
        OneofReader<T> ret;
        ret.Add(a1);
        ret.Add(a2);
        ret.Add(a3);
        ret.Add(a4);
        ret.Add(a5);
        ret.Add(a6);
        ret.Add(a7);
        ret.Add(a8);
        return ret;
    }

    template <class T>
    OneofReader<T> OneOf(T a1, T a2, T a3, T a4, T a5, T a6, T a7, T a8, T a9)
    {
        OneofReader<T> ret;
        ret.Add(a1);
        ret.Add(a2);
        ret.Add(a3);
        ret.Add(a4);
        ret.Add(a5);
        ret.Add(a6);
        ret.Add(a7);
        ret.Add(a8);
        ret.Add(a9);
        return ret;
    }

    template <class T>
    OneofReader<T> OneOf(T a1, T a2, T a3, T a4, T a5, T a6, T a7, T a8, T a9, T a10)
    {
        OneofReader<T> ret;
        ret.Add(a1);
        ret.Add(a2);
        ret.Add(a3);
        ret.Add(a4);
        ret.Add(a5);
        ret.Add(a6);
        ret.Add(a7);
        ret.Add(a8);
        ret.Add(a9);
        ret.Add(a10);
        return ret;
    }

    class Parser
    {
    public:
        Parser()
        {
        }

        ~Parser()
        {
            for (auto p = _options.begin(); p != _options.end(); ++p)
                delete p->second;
        }

        void Add(const std::string& name,
                 char short_name = 0,
                 const std::string& desc = "")
        {
            if (_options.count(name))
                throw CmdLineError("multiple definition: " + name);
            
            _options[name] = new OptionWithoutValue(name, short_name, desc);
            _ordered.push_back(_options[name]);
        }

        template <class T>
        void Add(const std::string& name,
                 char short_name = 0,
                 const std::string& desc = "",
                 bool need = true,
                 const T def = T())
        {
            Add(name, short_name, desc, need, def, DefaultReader<T>());
        }

        template <class T, class F>
        void Add(const std::string& name,
                 char short_name = 0,
                 const std::string& desc = "",
                 bool need = true,
                 const T def = T(),
                 F reader = F())
        {
            if (_options.count(name))
                throw CmdLineError("multiple definition: " + name);
            
            _options[name] = new OptionWithValueWithReader<T, F>(name, short_name, need, def, desc, reader);
            _ordered.push_back(_options[name]);
        }

        void Footer(const std::string& f)
        {
            _ftr = f;
        }

        void SetProgramName(const std::string& name)
        {
            _programmeName = name;
        }

        bool Exist(const std::string& name) const
        {
            if (_options.count(name) == 0)
                throw CmdLineError("there is no flag: --" + name);
            
            return _options.find(name)->second->HasSet();
        }

        template <class T>
        const T& Get(const std::string& name) const
        {
            if (_options.count(name) == 0)
                throw CmdLineError("there is no flag: --" + name);
            
            const OptionWithValue<T>* p = dynamic_cast<const OptionWithValue<T>*>(_options.find(name)->second);
            if (p == nullptr)
                throw CmdLineError("type mismatch flag '" + name + "'");
            
            return p->Get();
        }

        const std::vector<std::string>& Reset() const
        {
            return _others;
        }

        bool Parse(const std::string& arg)
        {
            std::vector<std::string> args;

            std::string buf;
            bool in_quote = false;
            for (std::string::size_type i = 0; i < arg.length(); i++)
            {
                if (arg[i] == '\"')
                {
                    in_quote = !in_quote;
                    continue;
                }

                if (arg[i] == ' ' && !in_quote)
                {
                    args.push_back(buf);
                    buf = "";
                    continue;
                }

                if (arg[i] == '\\')
                {
                    i++;
                    if (i >= arg.length())
                    {
                        _errors.push_back("unexpected occurrence of '\\' at end of string");
                        return false;
                    }
                }

                buf += arg[i];
            }

            if (in_quote)
            {
                _errors.push_back("quote is not closed");
                return false;
            }

            if (buf.length() > 0)
                args.push_back(buf);

            for (size_t i = 0; i < args.size(); i++)
                std::cout << "\"" << args[i] << "\"" << std::endl;

            return Parse(args);
        }

        bool Parse(const std::vector<std::string>& args)
        {
            int argc = static_cast<int>(args.size());
            std::vector<const char*> argv(argc);

            for (int i = 0; i < argc; i++)
                argv[i] = args[i].c_str();

            return Parse(argc, &argv[0]);
        }

        bool Parse(int argc, const char* const argv[])
        {
            _errors.clear();
            _others.clear();

            if (argc < 1)
            {
                _errors.push_back("argument number must be longer than 0");
                return false;
            }
            if (_programmeName == "")
                _programmeName = argv[0];

            std::map<char, std::string> lookup;
            for (auto p = _options.begin(); p != _options.end(); ++p)
            {
                if (p->first.length() == 0)
                    continue;
                
                char initial = p->second->ShortName();
                if (initial)
                {
                    if (lookup.count(initial) > 0)
                    {
                        lookup[initial] = "";
                        _errors.push_back(std::string("short option '") + initial + "' is ambiguous");
                        return false;
                    }
                    lookup[initial] = p->first;
                }
            }

            for (int i = 1; i < argc; i++)
            {
                if (strncmp(argv[i], "--", 2) == 0)
                {
                    const char* p = strchr(argv[i] + 2, '=');
                    if (p)
                    {
                        std::string name(argv[i] + 2, p);
                        std::string val(p + 1);
                        SetOption(name, val);
                    }
                    else
                    {
                        std::string name(argv[i] + 2);
                        if (_options.count(name) == 0)
                        {
                            _errors.push_back("undefined option: --" + name);
                            continue;
                        }
                        if (_options[name]->HasValue())
                        {
                            if (i + 1 >= argc)
                            {
                                _errors.push_back("option needs value: --" + name);
                                continue;
                            }
                            i++;
                            SetOption(name, argv[i]);
                        }
                        else
                        {
                            SetOption(name);
                        }
                    }
                }
                else if (strncmp(argv[i], "-", 1) == 0)
                {
                    if (!argv[i][1]) continue;
                    char last = argv[i][1];
                    for (int j = 2; argv[i][j]; j++)
                    {
                        last = argv[i][j];
                        if (lookup.count(argv[i][j - 1]) == 0)
                        {
                            _errors.push_back(std::string("undefined short option: -") + argv[i][j - 1]);
                            continue;
                        }
                        if (lookup[argv[i][j - 1]] == "")
                        {
                            _errors.push_back(std::string("ambiguous short option: -") + argv[i][j - 1]);
                            continue;
                        }
                        SetOption(lookup[argv[i][j - 1]]);
                    }

                    if (lookup.count(last) == 0)
                    {
                        _errors.push_back(std::string("undefined short option: -") + last);
                        continue;
                    }
                    if (lookup[last] == "")
                    {
                        _errors.push_back(std::string("ambiguous short option: -") + last);
                        continue;
                    }

                    if (i + 1 < argc && _options[lookup[last]]->HasValue())
                    {
                        SetOption(lookup[last], argv[i + 1]);
                        i++;
                    }
                    else
                    {
                        SetOption(lookup[last]);
                    }
                }
                else
                {
                    _others.push_back(argv[i]);
                }
            }

            for (auto p = _options.begin(); p != _options.end(); ++p)
                if (!p->second->Valid())
                    _errors.push_back("need option: --" + std::string(p->first));

            return _errors.size() == 0;
        }

        void ParseCheck(const std::string& arg)
        {
            if (!_options.count("help"))
                Add("help", '?', "print this message");
            
            Check(0, Parse(arg));
        }

        void ParseCheck(const std::vector<std::string>& args)
        {
            if (!_options.count("help"))
                Add("help", '?', "print this message");
            
            Check(args.size(), Parse(args));
        }

        void ParseCheck(int argc, char* argv[])
        {
            if (!_options.count("help"))
                Add("help", '?', "print this message");
            
            Check(argc, Parse(argc, argv));
        }

        std::string Error() const
        {
            return _errors.size() > 0 ? _errors[0] : "";
        }

        std::string ErrorFull() const
        {
            std::ostringstream oss;
            for (size_t i = 0; i < _errors.size(); i++)
                oss << _errors[i] << std::endl;
            return oss.str();
        }

        std::string Usage() const
        {
            std::ostringstream oss;
            oss << "usage: " << _programmeName << " ";
            for (size_t i = 0; i < _ordered.size(); i++)
            {
                if (_ordered[i]->Must())
                    oss << _ordered[i]->ShortDescription() << " ";
            }

            oss << "[options] ... " << _ftr << std::endl;
            oss << "options:" << std::endl;

            size_t max_width = 0;
            for (size_t i = 0; i < _ordered.size(); i++)
            {
                max_width = std::max(max_width, _ordered[i]->Name().length());
            }
            for (size_t i = 0; i < _ordered.size(); i++)
            {
                if (_ordered[i]->ShortName())
                {
                    oss << "  -" << _ordered[i]->ShortName() << ", ";
                }
                else
                {
                    oss << "      ";
                }

                oss << "--" << _ordered[i]->Name();
                for (size_t j = _ordered[i]->Name().length(); j < max_width + 4; j++)
                    oss << ' ';
                oss << _ordered[i]->Description() << std::endl;
            }
            return oss.str();
        }

    private:
        void Check(int argc, bool ok) const
        {
            if ((argc == 1 && !ok) || Exist("help"))
            {
                std::cerr << Usage();
                exit(0);
            }

            if (!ok)
            {
                std::cerr << Error() << std::endl << Usage();
                exit(1);
            }
        }

        void SetOption(const std::string& name)
        {
            if (_options.count(name) == 0)
            {
                _errors.push_back("undefined option: --" + name);
                return;
            }
            if (!_options[name]->Set())
            {
                _errors.push_back("option needs value: --" + name);
            }
        }

        void SetOption(const std::string& name, const std::string& value)
        {
            if (_options.count(name) == 0)
            {
                _errors.push_back("undefined option: --" + name);
                return;
            }
            if (!_options[name]->Set(value))
            {
                _errors.push_back("option value is invalid: --" + name + "=" + value);
            }
        }

    private:
        class OptionBase
        {
        public:
            virtual ~OptionBase()
            {
            }

            virtual bool HasValue() const =0;
            virtual bool Set() =0;
            virtual bool Set(const std::string& value) =0;
            virtual bool HasSet() const =0;
            virtual bool Valid() const =0;
            virtual bool Must() const =0;

            virtual const std::string& Name() const =0;
            virtual char ShortName() const =0;
            virtual const std::string& Description() const =0;
            virtual std::string ShortDescription() const =0;
        };

        class OptionWithoutValue : public OptionBase
        {
        public:
            OptionWithoutValue(const std::string& name,
                                 char short_name,
                                 const std::string& desc)
                : _name(name), _shortName(short_name), _desc(desc), _has(false)
            {
            }

            ~OptionWithoutValue() override
            {
            }

            bool HasValue() const override { return false; }

            bool Set() override
            {
                _has = true;
                return true;
            }

            bool Set(const std::string&) override
            {
                return false;
            }

            bool HasSet() const override
            {
                return _has;
            }

            bool Valid() const override
            {
                return true;
            }

            bool Must() const override
            {
                return false;
            }

            const std::string& Name() const override
            {
                return _name;
            }

            char ShortName() const override
            {
                return _shortName;
            }

            const std::string& Description() const override
            {
                return _desc;
            }

            std::string ShortDescription() const override
            {
                return "--" + _name;
            }

        private:
            std::string _name;
            char _shortName;
            std::string _desc;
            bool _has;
        };

        template <class T>
        class OptionWithValue : public OptionBase
        {
        public:
            OptionWithValue(const std::string& name,
                              char short_name,
                              bool need,
                              const T& def,
                              const std::string& desc)
                : _name(name), _shortName(short_name), _need(need), _has(false)
                  , _def(def), _actual(def)
            {
                this->_desc = FullDescription(desc);
            }

            ~OptionWithValue() override
            {
            }

            const T& Get() const
            {
                return _actual;
            }

            bool HasValue() const override { return true; }

            bool Set() override
            {
                return false;
            }

            bool Set(const std::string& value) override
            {
                try
                {
                    _actual = Read(value);
                    _has = true;
                }
                catch (const std::exception& e)
                {
                    return false;
                }
                return true;
            }

            bool HasSet() const override
            {
                return _has;
            }

            bool Valid() const override
            {
                if (_need && !_has) return false;
                return true;
            }

            bool Must() const override
            {
                return _need;
            }

            const std::string& Name() const override
            {
                return _name;
            }

            char ShortName() const override
            {
                return _shortName;
            }

            const std::string& Description() const override
            {
                return _desc;
            }

            std::string ShortDescription() const override
            {
                return "--" + _name + "=" + detail::readable_typename<T>();
            }

        protected:
            std::string FullDescription(const std::string& desc)
            {
                return
                    desc + " (" + detail::readable_typename<T>() +
                    (_need ? "" : " [=" + detail::default_value<T>(_def) + "]")
                    + ")";
            }

            virtual T Read(const std::string& s) =0;

            std::string _name;
            char _shortName;
            bool _need;
            std::string _desc;

            bool _has;
            T _def;
            T _actual;
        };

        template <class T, class F>
        class OptionWithValueWithReader : public OptionWithValue<T>
        {
        public:
            OptionWithValueWithReader(const std::string& name,
                                          char short_name,
                                          bool need,
                                          const T def,
                                          const std::string& desc,
                                          F reader)
                : OptionWithValue<T>(name, short_name, need, def, desc), _reader(reader)
            {
            }

        private:
            T Read(const std::string& s) override
            {
                return _reader(s);
            }

            F _reader;
        };

    private:
        std::map<std::string, OptionBase*> _options;
        std::vector<OptionBase*> _ordered;
        std::string _ftr;

        std::string _programmeName;
        std::vector<std::string> _others;

        std::vector<std::string> _errors;
    };
} // cmdline
