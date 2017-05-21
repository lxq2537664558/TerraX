#pragma once

namespace terra
{
    class IModule
    {
    public:
        IModule() = default;
        virtual ~IModule() {}

        virtual bool Init() { return true; }

        virtual bool AfterInit() { return true; }

		virtual bool Execute() { return true; }

        virtual bool BeforeShut() { return true; }

        virtual bool Shut() { return true; }

    };
}