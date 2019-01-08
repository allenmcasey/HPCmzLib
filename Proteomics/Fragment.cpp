﻿#include "Fragment.h"
#include "AminoAcidPolymer.h"
#include "Terminus.h"
#include "../MzLibUtil/MzLibUtil.h"

using namespace Chemistry;

namespace Proteomics {

    Fragment::Fragment(FragmentTypes type, int number, double monoisotopicMass, AminoAcidPolymer *parent) {
        setFragmentType(type);
        setNumber(number);
        setParent(parent);
        setMonoisotopicMass(monoisotopicMass);
    }

    double Fragment::getMonoisotopicMass() const {
        return privateMonoisotopicMass;
    }

    void Fragment::setMonoisotopicMass(double value) {
        privateMonoisotopicMass = value;
    }

    int Fragment::getNumber() const {
        return privateNumber;
    }

    void Fragment::setNumber(int value) {
        privateNumber = value;
    }

    AminoAcidPolymer *Fragment::getParent() const {
        return privateParent;
    }

    void Fragment::setParent(AminoAcidPolymer *value) {
        privateParent = value;
    }

    FragmentTypes Fragment::getFragmentType() const {
        return privateFragmentType;
    }

    void Fragment::setFragmentType(FragmentTypes value) {
        privateFragmentType = value;
    }

    std::vector<IHasMass*> Fragment::getModifications() const {
        auto mods = getParent()->getModifications();
        if (getFragmentType()::GetTerminus() == Terminus::N) {
            for (int i = 0; i <= getNumber(); i++) {
                if (mods[i] != nullptr) {
//C# TO C++ CONVERTER TODO TASK: C++ does not have an equivalent to the C# 'yield' keyword:
                    yield return mods[i];
                }
            }
        }
        else {
            int length = getParent()->getLength() + 1;
            for (int i = length - getNumber(); i <= length; i++) {
                if (mods[i] != nullptr) {
//C# TO C++ CONVERTER TODO TASK: C++ does not have an equivalent to the C# 'yield' keyword:
                    yield return mods[i];
                }
            }
        }
    }

    std::string Fragment::getSequence() const {
        std::string parentSeq = getParent()->getBaseSequence();
        if (getFragmentType()::GetTerminus() == Terminus::N) {
            return parentSeq.substr(0, getNumber());
        }

        return parentSeq.substr(parentSeq.length() - getNumber(), getNumber());
    }

    std::string Fragment::ToString() {
        return std::string::Format(CultureInfo::InvariantCulture, "{0}{1}", Enum::GetName(FragmentTypes::typeid, getFragmentType()), getNumber());
    }

    int Fragment::GetHashCode() {
        return getMonoisotopicMass().GetHashCode();
    }

    bool Fragment::Equals(Fragment *other) {
        return getFragmentType().Equals(other->getFragmentType()) && getNumber().Equals(other->getNumber()) && std::abs(getMonoisotopicMass() - other->getMonoisotopicMass()) < 1e-9;
    }
}
