//
// Base classes for function pointers that the state handlers will inherit
//
#pragma once
#include "context.h"
#include "utility.h"

namespace UriParser
{
    typedef bool (*RawBoolFunc)(Context &);
    typedef void (*RawVoidFunc)(Context &);

    class Func: private NonCopyable
    {
        public:
            Func()
            {
            }

            virtual ~Func()
            {
            }
    };

    class BoolFunc : public Func
    {
        protected:
            virtual bool Act(Context& context) = 0;
        public:
            bool operator()(Context& context)
            {
                return Act(context);
            }
    };

    class VoidFunc : public Func
    {
    protected:
        virtual void Act(Context& context) = 0;
    public:
        void operator()(Context& context)
        {
            Act(context);
        }
    };

    class StubVoidFunc : public VoidFunc
    {
    public:
        static StubVoidFunc &Instance()
        {
            static StubVoidFunc func;
            return func;
        }
    protected:
        virtual void Act(Context&) override
        {
        }
    };

    class TrueBoolFunc : public BoolFunc
    {
    public:
        static TrueBoolFunc &Instance()
        {
            static TrueBoolFunc func;
            return func;
        }
    protected:
        virtual bool Act(Context &) override
        {
            return true;
        }
    };

    // Always returns false
    class FalseBoolFunc : public BoolFunc
    {
        public:
            static FalseBoolFunc &Instance()
            {
                static FalseBoolFunc func;
                return func;
            }
        protected:
            virtual bool Act(Context &) override
            {
                return false;
            }
    };

    class BoolFuncAdaptor : public BoolFunc
    {
        private:
            RawBoolFunc func_;
        public:
            BoolFuncAdaptor(RawBoolFunc raw_func):
                func_(raw_func)
            {
            }
        protected:
            virtual bool Act(Context &context)
            {
                return func_(context);
            }
    };

    class VoidFuncAdaptor : public VoidFunc
    {
        private:
            RawVoidFunc func_;
        public:
            VoidFuncAdaptor(RawVoidFunc raw_func):
                func_(raw_func)
            {
            }
        protected:
            virtual void Act(Context &context)
            {
                func_(context);
            }
    };
}
