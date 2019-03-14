﻿#pragma once

#include "Modification.h"
#include "TerminusLocalization.h"
#include <string>
#include <unordered_map>
#include <vector>
#include <any>
#include "stringbuilder.h"

#include "ModificationMotif.h"


namespace Proteomics {
    class ModificationWithLocation : public Modification {

    public:
        static std::unordered_map<std::string, TerminusLocalization> terminusLocalizationTypeCodes;
        std::unordered_map<std::string, std::vector<std::string>> linksToOtherDbs;
        std::vector<std::string> keywords;
        const TerminusLocalization terminusLocalization;
        ModificationMotif *motif;

        virtual ~ModificationWithLocation() {
            delete motif;
        }

    private:
        class StaticConstructor {
        public:
            StaticConstructor();
        };

    private:
        static ModificationWithLocation::StaticConstructor staticConstructor;


    public:
        ModificationWithLocation(const std::string &id, const std::string &modificationType,
                                 ModificationMotif *motif,
                                 TerminusLocalization terminusLocalization,
                                 std::unordered_map<std::string, std::vector<std::string>> *linksToOtherDbs,
                                 std::vector<std::string> *keywords);

        
        std::string ToString();
        bool Equals(ModificationWithLocation *m);
        int GetHashCode();

    };
}
