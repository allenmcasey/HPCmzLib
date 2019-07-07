﻿#include "Modification.h"
#include "ModificationMotif.h"
#include "../../Chemistry/ChemicalFormula.h"
#include "../../Chemistry/ClassExtensions.h"

using namespace Chemistry;
using namespace MassSpectrometry;

namespace Proteomics
{

    std::string Modification::getIdWithMotif() const
    {
        return privateIdWithMotif;
    }

    void Modification::setIdWithMotif(const std::string &value)
    {
        privateIdWithMotif = value;
    }

    std::string Modification::getOriginalId() const
    {
        return privateOriginalId;
    }

    void Modification::setOriginalId(const std::string &value)
    {
        privateOriginalId = value;
    }

    std::string Modification::getAccession() const
    {
        return privateAccession;
    }

    void Modification::setAccession(const std::string &value)
    {
        privateAccession = value;
    }

    std::string Modification::getModificationType() const
    {
        return privateModificationType;
    }

    void Modification::setModificationType(const std::string &value)
    {
        privateModificationType = value;
    }

    std::string Modification::getFeatureType() const
    {
        return privateFeatureType;
    }

    void Modification::setFeatureType(const std::string &value)
    {
        privateFeatureType = value;
    }

    ModificationMotif *Modification::getTarget() const
    {
        return privateTarget;
    }

    void Modification::setTarget(ModificationMotif *value)
    {
        privateTarget = value;
    }

    std::string Modification::getLocationRestriction() const
    {
        return privateLocationRestriction;
    }

    void Modification::setLocationRestriction(const std::string &value)
    {
        privateLocationRestriction = value;
    }

    Chemistry::ChemicalFormula *Modification::getChemicalFormula() const
    {
        return privateChemicalFormula;
    }

    void Modification::setChemicalFormula(Chemistry::ChemicalFormula *value)
    {
        privateChemicalFormula = value;
    }

    std::optional<double> Modification::getMonoisotopicMass() const
    {
        return ClassExtensions::RoundedDouble(monoisotopicMass);
    }

    void Modification::setMonoisotopicMass(const std::optional<double> &value)
    {
        monoisotopicMass = value;
    }

    std::unordered_map<std::string, std::vector<std::string>> Modification::getDatabaseReference() const
    {
        return privateDatabaseReference;
    }

    void Modification::setDatabaseReference(const std::unordered_map<std::string, std::vector<std::string>> &value)
    {
        privateDatabaseReference = value;
    }

    std::unordered_map<std::string, std::vector<std::string>> Modification::getTaxonomicRange() const
    {
        return privateTaxonomicRange;
    }

    void Modification::setTaxonomicRange(const std::unordered_map<std::string, std::vector<std::string>> &value)
    {
        privateTaxonomicRange = value;
    }

    std::vector<std::string> Modification::getKeywords() const
    {
        return privateKeywords;
    }

    void Modification::setKeywords(const std::vector<std::string> &value)
    {
        privateKeywords = value;
    }

    std::unordered_map<DissociationType, std::vector<double>> Modification::getNeutralLosses() const
    {
        return privateNeutralLosses;
    }

    void Modification::setNeutralLosses(const std::unordered_map<DissociationType, std::vector<double>> &value)
    {
        privateNeutralLosses = value;
    }

    std::unordered_map<DissociationType, std::vector<double>> Modification::getDiagnosticIons() const
    {
        return privateDiagnosticIons;
    }

    void Modification::setDiagnosticIons(const std::unordered_map<DissociationType, std::vector<double>> &value)
    {
        privateDiagnosticIons = value;
    }

    std::string Modification::getFileOrigin() const
    {
        return privateFileOrigin;
    }

    void Modification::setFileOrigin(const std::string &value)
    {
        privateFileOrigin = value;
    }

    bool Modification::getValidModification() const
    {
        return this->getIdWithMotif() != "" && (this->getChemicalFormula() != nullptr || this->getMonoisotopicMass()) && this->getTarget() != nullptr && this->getLocationRestriction() != "Unassigned." && this->getModificationType() != "" && this->getFeatureType() != "CROSSLINK" && !this->getModificationType().find(':') != std::string::npos;
    }

    Modification::Modification(const std::string &_originalId, const std::string &_accession, const std::string &_modificationType, const std::string &_featureType, ModificationMotif *_target, const std::string &_locationRestriction, Chemistry::ChemicalFormula *_chemicalFormula, std::optional<double> &_monoisotopicMass, std::unordered_map<std::string, std::vector<std::string>> &_databaseReference, std::unordered_map<std::string, std::vector<std::string>> &_taxonomicRange, std::vector<std::string> &_keywords, std::unordered_map<DissociationType, std::vector<double>> &_neutralLosses, std::unordered_map<DissociationType, std::vector<double>> &_diagnosticIons, const std::string &_fileOrigin)
    {
        if (_originalId != "")
        {
            if (_originalId.find(" on ") != std::string::npos)
            {
                this->setIdWithMotif(_originalId);
                this->setOriginalId(_originalId.Split({" on "}, StringSplitOptions::None)[0]);
            }
            else if (_originalId.find(" of ") != std::string::npos)
            {
                this->setIdWithMotif(StringHelper::replace(_originalId, " of ", " on "));
                this->setOriginalId(_originalId.Split({" of ", " on "}, StringSplitOptions::None)[0]);
            }
            else if (_target != nullptr)
            {
//C# TO C++ CONVERTER TODO TASK: There is no C++ equivalent to 'ToString':
                this->setIdWithMotif(_originalId + " on " + _target->ToString());
                this->setOriginalId(_originalId);
            }
            else
            {
                this->setOriginalId(_originalId);
            }
        }

        this->setAccession(_accession);
        this->setModificationType(_modificationType);
        this->setFeatureType(_featureType);
        this->setTarget(_target);
        this->setLocationRestriction(ModLocationOnPeptideOrProtein(_locationRestriction));
        this->setChemicalFormula(_chemicalFormula);
        this->setMonoisotopicMass(_monoisotopicMass);
        this->setDatabaseReference(_databaseReference);
        this->setTaxonomicRange(_taxonomicRange);
        this->setKeywords(_keywords);
        this->setNeutralLosses(_neutralLosses);
        this->setDiagnosticIons(_diagnosticIons);
        this->setFileOrigin(_fileOrigin);

        if (!this->getMonoisotopicMass() && this->getChemicalFormula() != nullptr)
        {
            this->setMonoisotopicMass(std::make_optional(this->getChemicalFormula()->getMonoisotopicMass()));
        }
    }

    std::string Modification::ModLocationOnPeptideOrProtein(const std::string &_locationRestriction)
    {
//C# TO C++ CONVERTER NOTE: The following 'switch' operated on a string variable and was converted to C++ 'if-else' logic:
//        switch (_locationRestriction)
//ORIGINAL LINE: case "N-terminal.":
        if (_locationRestriction == "N-terminal.")
        {
                return _locationRestriction;

        }
//ORIGINAL LINE: case "C-terminal.":
        else if (_locationRestriction == "C-terminal.")
        {
                return _locationRestriction;

        }
//ORIGINAL LINE: case "Peptide N-terminal.":
        else if (_locationRestriction == "Peptide N-terminal.")
        {
                return _locationRestriction;

        }
//ORIGINAL LINE: case "Peptide C-terminal.":
        else if (_locationRestriction == "Peptide C-terminal.")
        {
                return _locationRestriction;

        }
//ORIGINAL LINE: case "Anywhere.":
        else if (_locationRestriction == "Anywhere.")
        {
                return _locationRestriction;

        }
        else
        {
                return "Unassigned.";
        }
    }

    bool Modification::Equals(std::any o)
    {
        Modification *m = dynamic_cast<Modification*>(o);
        return o.has_value() && getIdWithMotif() == m->getIdWithMotif() && getOriginalId() == m->getOriginalId() && getModificationType() == m->getModificationType() && (getMonoisotopicMass() == m->getMonoisotopicMass() || getMonoisotopicMass() && m->getMonoisotopicMass() && std::abs(static_cast<double>(m->getMonoisotopicMass()) - static_cast<double>(getMonoisotopicMass())) < tolForEquality);
    }

    int Modification::GetHashCode()
    {
        std::string tempVar = getIdWithMotif();
        std::string tempVar2 = getOriginalId();
        std::string id = (tempVar != nullptr) ? tempVar : (tempVar2 != nullptr) ? tempVar2 : "";
        std::string tempVar3 = getModificationType();
        std::string mt = (tempVar3 != nullptr) ? tempVar3 : "";
        return id.GetHashCode() ^ mt.GetHashCode();
    }

    std::string Modification::ToString()
    {
        StringBuilder *sb = new StringBuilder();
        if (this->getIdWithMotif() != "")
        {
            sb->appendLine("ID   " + this->getIdWithMotif());
        }
        if (this->getAccession() != "")
        {
            sb->appendLine("AC   " + this->getAccession());
        }
        if (this->getModificationType() != "")
        {
            sb->appendLine("MT   " + this->getModificationType());
        }
        if (this->getFeatureType() != "")
        {
            sb->appendLine("FT   " + this->getFeatureType());
        }
        if (this->getTarget() != nullptr)
        {
            sb->appendLine("TG   " + this->getTarget());
        } // at this stage, each mod has only one target though many may have the same Id
        if (this->getLocationRestriction() != "")
        {
            sb->appendLine("PP   " + this->getLocationRestriction());
        }
        if (this->getChemicalFormula() != nullptr)
        {
            sb->appendLine("CF   " + this->getChemicalFormula()->getFormula());
        }
        if (this->getMonoisotopicMass())
        {
            sb->appendLine("MM   " + this->getMonoisotopicMass());
        }
        if (!this->getDatabaseReference().empty())
        {
            if (this->getDatabaseReference().size() != 0)
            {
                std::vector<std::string> myKeys(this->getDatabaseReference().Keys);
                std::sort(myKeys.begin(), myKeys.end());
                for (auto myKey : myKeys)
                {
                    std::vector<std::string> myValues(this->getDatabaseReference()[myKey]);
                    std::sort(myValues.begin(), myValues.end());
                    for (auto myValue : myValues)
                    {
                        sb->appendLine("DR   " + myKey + "; " + myValue);
                    }
                }
            }
        }
        if (!this->getTaxonomicRange().empty())
        {
            if (this->getTaxonomicRange().size() != 0)
            {
                std::vector<std::string> myKeys(this->getTaxonomicRange().Keys);
                std::sort(myKeys.begin(), myKeys.end());
                for (auto myKey : myKeys)
                {
                    std::vector<std::string> myValues(this->getTaxonomicRange()[myKey]);
                    std::sort(myValues.begin(), myValues.end());
                    for (auto myValue : myValues)
                    {
                        sb->appendLine("TR   " + myKey + "; " + myValue);
                    }
                }
            }
        }
        if (!this->getNeutralLosses().empty())
        {
            if (this->getNeutralLosses().size() != 0)
            {
                std::vector<DissociationType> allDissociationTypes = this->getNeutralLosses().Keys->ToList();
                std::sort(allDissociationTypes.begin(), allDissociationTypes.end());

                for (auto dissociationType : allDissociationTypes)
                {
                    StringBuilder *myLine = new StringBuilder();
                    myLine->append("NL   ");

                    std::vector<double> myValues(this->getNeutralLosses()[dissociationType]);
                    std::sort(myValues.begin(), myValues.end());
                    for (int i = 0; i < myValues.size(); i++)
                    {
                        myLine->append(dissociationType + ":" + ClassExtensions::RoundedDouble(std::make_optional(myValues[i])));
                        if (i < myValues.size() - 1)
                        {
                            myLine->append(" or ");
                        }
                    }

                    sb->appendLine(myLine->toString());

                    delete myLine;
                }
            }
        }
        if (!this->getDiagnosticIons().empty())
        {
            if (this->getDiagnosticIons().size() != 0)
            {
                std::vector<DissociationType> allDissociationTypes = this->getDiagnosticIons().Keys->ToList();
                std::sort(allDissociationTypes.begin(), allDissociationTypes.end());

                for (auto dissociationType : allDissociationTypes)
                {
                    StringBuilder *myLine = new StringBuilder();
                    myLine->append("DI   ");

                    std::vector<double> myValues(this->getDiagnosticIons()[dissociationType]);
                    std::sort(myValues.begin(), myValues.end());
                    for (int i = 0; i < myValues.size(); i++)
                    {
                        myLine->append(dissociationType + ":" + ClassExtensions::RoundedDouble(std::make_optional(myValues[i])));
                        if (i < myValues.size() - 1)
                        {
                            myLine->append(" or ");
                        }
                    }

                    sb->appendLine(myLine->toString());

                    delete myLine;
                }
            }
        }

        if (!this->getKeywords().empty())
        {
            if (this->getKeywords().size() != 0)
            {
                sb->appendLine("KW   " + std::string::Join(" or ", this->getKeywords().ToList()->OrderBy([&] (std::any b)
                {
                delete sb;
                    return b;
                })));
            }
        }

        delete sb;
        return sb->toString();
    }

    std::string Modification::ModificationErrorsToString()
    {
        StringBuilder *sb = new StringBuilder();

//C# TO C++ CONVERTER TODO TASK: There is no C++ equivalent to 'ToString':
        sb->append(this->ToString());

        if (this->getIdWithMotif() == "")
        {
            sb->appendLine("#Required field ID missing or malformed. Current value = " + this->getIdWithMotif());
        }

        if (this->getModificationType() == "")
        {
            sb->appendLine("#Required field MT missing or malformed. Current value = " + this->getModificationType());
        }

        if (this->getLocationRestriction() == "")
        {
            sb->appendLine("#Required field PP missing or malformed. Current value = " + this->getLocationRestriction() + ".");
        }

        if (this->getChemicalFormula() == nullptr && !this->getMonoisotopicMass())
        {
            sb->appendLine("#Required fields CF and MM are both missing or malformed. One of those two fields must be provided.");
        }

        if (this->getModificationType() != "" && this->getModificationType().find(':') != std::string::npos)
        {
            sb->appendLine("#Modification type cannot contain ':'!");
        }

        sb->append("#This modification can be found in file " + this->getFileOrigin());

        delete sb;
        return sb->toString();
    }
}