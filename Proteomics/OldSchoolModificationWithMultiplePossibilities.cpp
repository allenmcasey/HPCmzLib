﻿#include "OldSchoolModificationWithMultiplePossibilities.h"
#include "../MzLibUtil/MzLibException.h"

#include "MzLibUtil.h"
using namespace MzLibUtil;

namespace Proteomics {

    ModificationWithMultiplePossibilitiesCollection::ModificationWithMultiplePossibilitiesCollection(const std::string &name, ModificationSites sites) : OldSchoolModification(0, name, sites), _modifications(new std::map<double, OldSchoolModification*>()) {
    }

    int ModificationWithMultiplePossibilitiesCollection::getCount() const {
        return _modifications->size();
    }

    OldSchoolModification *ModificationWithMultiplePossibilitiesCollection::operator [](int index) {
        return _modifications->at(index);
    }

    void ModificationWithMultiplePossibilitiesCollection::AddModification(OldSchoolModification *modification) {
#if ORIG
        if (!getSites()::ContainsSites(modification->getSites())) {
            throw MzLibException("Unable to add a modification with sites other than " + getSites());
        }
#endif
        double key=modification->getMonoisotopicMass();
        _modifications->insert(std::pair<double, OldSchoolModification*>(key, modification));
    }

    bool ModificationWithMultiplePossibilitiesCollection::Contains(OldSchoolModification *modification) {
        std::map<double, OldSchoolModification*>::const_iterator it;
        for (  it = _modifications->begin(); it != _modifications->end(); it++ ){
            if ( it->second == modification){
                return true;
            }
        }
        return false;
    }

//    IEnumerator<OldSchoolModification*> *ModificationWithMultiplePossibilitiesCollection::GetEnumerator() {
//        return _modifications->Values->begin();
//    }
//
//    System::Collections::IEnumerator *ModificationWithMultiplePossibilitiesCollection::IEnumerable_GetEnumerator() {
//        return _modifications->Values->begin();
//    }
}
