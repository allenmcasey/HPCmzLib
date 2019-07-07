﻿#include "Protease.h"
#include "DigestionMotif.h"
#include "../Protein/Protein.h"
#include "ProteolyticPeptide.h"

namespace Proteomics
{
    namespace ProteolyticDigestion
    {

        Protease::Protease(const std::string &name, Proteomics::ProteolyticDigestion::CleavageSpecificity cleavageSpecificity, const std::string &psiMSAccessionNumber, const std::string &psiMSName, std::vector<DigestionMotif*> &motifList)
        {
            Name = name;
            CleavageSpecificity = cleavageSpecificity;
            PsiMsAccessionNumber = psiMSAccessionNumber;
            PsiMsName = psiMSName;
            DigestionMotifs = motifList ? motifList : std::vector<DigestionMotif*>();
        }

        std::string Protease::getName() const
        {
            return privateName;
        }

        Proteomics::ProteolyticDigestion::CleavageSpecificity Protease::getCleavageSpecificity() const
        {
            return privateCleavageSpecificity;
        }

        std::string Protease::getPsiMsAccessionNumber() const
        {
            return privatePsiMsAccessionNumber;
        }

        std::string Protease::getPsiMsName() const
        {
            return privatePsiMsName;
        }

        std::vector<DigestionMotif*> Protease::getDigestionMotifs() const
        {
            return privateDigestionMotifs;
        }

        std::string Protease::ToString()
        {
            return getName();
        }

        bool Protease::Equals(std::any obj)
        {
            auto a = dynamic_cast<Protease*>(obj);
            return a != nullptr && (a->getName() == "" && getName() == "" || a->getName() == getName());
        }

        int Protease::GetHashCode()
        {
            std::string tempVar = getName();
            return ((tempVar != nullptr) ? tempVar : "")->GetHashCode();
        }

        Proteomics::ProteolyticDigestion::CleavageSpecificity Protease::GetCleavageSpecificity(const std::string &proteinSequence, int startIndex, int endIndex)
        {
            std::vector<int> indicesToCleave = GetDigestionSiteIndices(proteinSequence);
            int cleavableMatches = 0;
            //if the start index is a cleavable index (-1 because one based) OR if the start index is after a cleavable methionine
            if (std::find(indicesToCleave.begin(), indicesToCleave.end(), startIndex - 1) != indicesToCleave.end() || (startIndex == 2 && proteinSequence[0] == 'M'))
            {
                cleavableMatches++;
            }
            if (std::find(indicesToCleave.begin(), indicesToCleave.end(), endIndex) != indicesToCleave.end()) //if the end index is a cleavable index
            {
                cleavableMatches++;
            }

            if (cleavableMatches == 0 || getCleavageSpecificity() == getCleavageSpecificity()::SingleN || getCleavageSpecificity() == getCleavageSpecificity()::SingleC)
            {
                return getCleavageSpecificity()::None;
            }
            else if (cleavableMatches == 1) //if one index was cleavable, then it's semi specific
            {
                return getCleavageSpecificity()::Semi;
            }
            else //2 if both, then it's fully speific
            {
                return getCleavageSpecificity()::Full;
            }
        }

        std::vector<ProteolyticPeptide*> Protease::GetUnmodifiedPeptides(Protein *protein, int maximumMissedCleavages, InitiatorMethionineBehavior initiatorMethionineBehavior, int minPeptidesLength, int maxPeptidesLength)
        {
            std::vector<ProteolyticPeptide*> peptides;

            // proteolytic cleavage in one spot (N)
            if (getCleavageSpecificity() == getCleavageSpecificity()::SingleN)
            {
                bool maxTooBig = protein->getLength() + maxPeptidesLength < 0; //when maxPeptidesLength is too large, it becomes negative and causes issues
                //This happens when maxPeptidesLength == int.MaxValue or something close to it
                int startIndex = initiatorMethionineBehavior == InitiatorMethionineBehavior::Cleave ? 2 : 1;
                for (int proteinStart = startIndex; proteinStart <= protein->getLength(); proteinStart++)
                {
                    if (OkayMinLength(std::make_optional(protein->getLength() - proteinStart + 1), std::make_optional(minPeptidesLength)))
                    {
                        //need Math.Max if max length is int.MaxLength, since +proteinStart will make it negative
                        //if the max length is too big to be an int (ie infinity), just do the protein length.
                        //if it's not too big to be an int, it might still be too big. Take the minimum of the protein length or the maximum length (-1, because the index is inclusive. Without -1, peptides will be one AA too long)
                        ProteolyticPeptide tempVar(protein, proteinStart, maxTooBig ? protein->getLength() : std::min(protein->getLength(), proteinStart + maxPeptidesLength - 1), 0, getCleavageSpecificity()::SingleN, "SingleN");
                        peptides.push_back(&tempVar);
                    }
                }
            }

            // proteolytic cleavage in one spot (C)
            else if (getCleavageSpecificity() == getCleavageSpecificity()::SingleC)
            {
                int startIndex = initiatorMethionineBehavior == InitiatorMethionineBehavior::Cleave ? 2 : 1; //where does the protein start
                int lengthDifference = startIndex - 1; //take it back one for zero based index
                for (int proteinEnd = 1; proteinEnd <= protein->getLength(); proteinEnd++)
                {
                    //length of peptide will be at least the start index
                    if (OkayMinLength(std::make_optional(proteinEnd - lengthDifference), std::make_optional(minPeptidesLength))) //is the maximum possible length longer than the minimum?
                    {
                        //use the start index as the max of the N-terminus or the c-terminus minus the max (+1 because inclusive, otherwise peptides will be one AA too long)
                        ProteolyticPeptide tempVar2(protein, std::max(startIndex, proteinEnd - maxPeptidesLength + 1), proteinEnd, 0, getCleavageSpecificity()::SingleC, "SingleC");
                        peptides.push_back(&tempVar2);
                    }
                }
            }

            //top-down
            else if (getCleavageSpecificity() == getCleavageSpecificity()::None)
            {
                // retain methionine
                if ((initiatorMethionineBehavior != InitiatorMethionineBehavior::Cleave || protein[0] != 'M') && OkayLength(std::make_optional(protein->getLength()), std::make_optional(minPeptidesLength), std::make_optional(maxPeptidesLength)))
                {
                    ProteolyticPeptide tempVar3(protein, 1, protein->getLength(), 0, getCleavageSpecificity()::Full, "full");
                    peptides.push_back(&tempVar3);
                }

                // cleave methionine
                if ((initiatorMethionineBehavior != InitiatorMethionineBehavior::Retain && protein[0] == 'M') && OkayLength(std::make_optional(protein->getLength() - 1), std::make_optional(minPeptidesLength), std::make_optional(maxPeptidesLength)))
                {
                    ProteolyticPeptide tempVar4(protein, 2, protein->getLength(), 0, getCleavageSpecificity()::Full, "full:M cleaved");
                    peptides.push_back(&tempVar4);
                }

                // Also digest using the proteolysis product start/end indices
                peptides.AddRange(protein->getProteolysisProducts().Where([&] (std::any proteolysisProduct)
                {
                    return proteolysisProduct::OneBasedEndPosition.HasValue && proteolysisProduct::OneBasedBeginPosition.HasValue && OkayLength(proteolysisProduct::OneBasedEndPosition->Value - proteolysisProduct::OneBasedBeginPosition->Value + 1, std::make_optional(minPeptidesLength), std::make_optional(maxPeptidesLength));
                })->Select([&] (std::any proteolysisProduct)
                {
                    new ProteolyticPeptide(protein, proteolysisProduct::OneBasedBeginPosition->Value, proteolysisProduct::OneBasedEndPosition->Value, 0, getCleavageSpecificity()::Full, proteolysisProduct::Type);
                }));
            }

            // Full proteolytic cleavage
            else if (getCleavageSpecificity() == getCleavageSpecificity()::Full)
            {
                peptides.AddRange(FullDigestion(protein, initiatorMethionineBehavior, maximumMissedCleavages, minPeptidesLength, maxPeptidesLength));
            }

            // Cleavage rules for semi-specific search
            else if (getCleavageSpecificity() == getCleavageSpecificity()::Semi)
            {
                peptides.AddRange(SemiProteolyticDigestion(protein, initiatorMethionineBehavior, maximumMissedCleavages, minPeptidesLength, maxPeptidesLength));
            }

            else
            {
                throw NotImplementedException();
            }

            return peptides;
        }

        std::vector<int> Protease::GetDigestionSiteIndices(const std::string &proteinSequence)
        {
            auto indices = std::vector<int>();
            for (int r = 0; r < proteinSequence.length(); r++)
            {
                for (auto motif : getDigestionMotifs())
                {
                    if (motif->Fits(proteinSequence, r) && r + motif->CutIndex < proteinSequence.length())
                    {
                        indices.push_back(r + motif->CutIndex);
                    }
                }
            }

            indices.push_back(0); // The start of the protein is treated as a cleavage site to retain the n-terminal peptide
            indices.push_back(proteinSequence.length()); // The end of the protein is treated as a cleavage site to retain the c-terminal peptide
            return indices.Distinct().OrderBy([&] (std::any i)
            {
                return i;
            }).ToList();
        }

        bool Protease::Retain(int oneBasedCleaveAfter, InitiatorMethionineBehavior initiatorMethionineBehavior, char nTerminus)
        {
            return oneBasedCleaveAfter != 0 || initiatorMethionineBehavior != InitiatorMethionineBehavior::Cleave || nTerminus != 'M';
        }

        bool Protease::Cleave(int oneBasedCleaveAfter, InitiatorMethionineBehavior initiatorMethionineBehavior, char nTerminus)
        {
            return oneBasedCleaveAfter == 0 && initiatorMethionineBehavior != InitiatorMethionineBehavior::Retain && nTerminus == 'M';
        }

        bool Protease::OkayLength(std::optional<int> &peptideLength, std::optional<int> &minPeptidesLength, std::optional<int> &maxPeptidesLength)
        {
            return OkayMinLength(peptideLength, minPeptidesLength) && OkayMaxLength(peptideLength, maxPeptidesLength);
        }

        std::vector<ProteolyticPeptide*> Protease::FullDigestion(Protein *protein, InitiatorMethionineBehavior initiatorMethionineBehavior, int maximumMissedCleavages, int minPeptidesLength, int maxPeptidesLength)
        {
            std::vector<int> oneBasedIndicesToCleaveAfter = GetDigestionSiteIndices(protein->getBaseSequence());
            for (int missedCleavages = 0; missedCleavages <= maximumMissedCleavages; missedCleavages++)
            {
                for (int i = 0; i < oneBasedIndicesToCleaveAfter.size() - missedCleavages - 1; i++)
                {
                    if (Retain(i, initiatorMethionineBehavior, protein[0]) && OkayLength(std::make_optional(oneBasedIndicesToCleaveAfter[i + missedCleavages + 1] - oneBasedIndicesToCleaveAfter[i]), std::make_optional(minPeptidesLength), std::make_optional(maxPeptidesLength)))
                    {
//C# TO C++ CONVERTER TODO TASK: C++ does not have an equivalent to the C# 'yield' keyword:
                        yield return new ProteolyticPeptide(protein, oneBasedIndicesToCleaveAfter[i] + 1, oneBasedIndicesToCleaveAfter[i + missedCleavages + 1], missedCleavages, getCleavageSpecificity()::Full, "full");
                    }
                    if (Cleave(i, initiatorMethionineBehavior, protein[0]) && OkayLength(std::make_optional(oneBasedIndicesToCleaveAfter[i + missedCleavages + 1] - 1), std::make_optional(minPeptidesLength), std::make_optional(maxPeptidesLength)))
                    {
//C# TO C++ CONVERTER TODO TASK: C++ does not have an equivalent to the C# 'yield' keyword:
                        yield return new ProteolyticPeptide(protein, 2, oneBasedIndicesToCleaveAfter[i + missedCleavages + 1], missedCleavages, getCleavageSpecificity()::Full, "full:M cleaved");
                    }
                }

                // Also digest using the proteolysis product start/end indices
                for (auto proteolysisProduct : protein->getProteolysisProducts())
                {
                    if (proteolysisProduct->getOneBasedBeginPosition() != 1 || proteolysisProduct->getOneBasedEndPosition() != protein->getLength())
                    {
                        int i = 0;
//C# TO C++ CONVERTER TODO TASK: Comparisons involving nullable type instances will need to be rewritten since comparison rules are different between C++ optional and System.Nullable:
                        while (oneBasedIndicesToCleaveAfter[i] < proteolysisProduct->getOneBasedBeginPosition())
                        {
                            i++;
                        }

//C# TO C++ CONVERTER TODO TASK: Comparisons involving nullable type instances will need to be rewritten since comparison rules are different between C++ optional and System.Nullable:
                        bool startPeptide = i + missedCleavages < oneBasedIndicesToCleaveAfter.size() && oneBasedIndicesToCleaveAfter[i + missedCleavages] <= proteolysisProduct->getOneBasedEndPosition() && proteolysisProduct->getOneBasedBeginPosition() && OkayLength(std::make_optional(oneBasedIndicesToCleaveAfter[i + missedCleavages] - proteolysisProduct->getOneBasedBeginPosition().value() + 1), std::make_optional(minPeptidesLength), std::make_optional(maxPeptidesLength));
                        if (startPeptide)
                        {
//C# TO C++ CONVERTER TODO TASK: C++ does not have an equivalent to the C# 'yield' keyword:
                            yield return new ProteolyticPeptide(protein, proteolysisProduct->getOneBasedBeginPosition().value(), oneBasedIndicesToCleaveAfter[i + missedCleavages], missedCleavages, getCleavageSpecificity()::Full, proteolysisProduct->getType() + " start");
                        }

//C# TO C++ CONVERTER TODO TASK: Comparisons involving nullable type instances will need to be rewritten since comparison rules are different between C++ optional and System.Nullable:
                        while (oneBasedIndicesToCleaveAfter[i] < proteolysisProduct->getOneBasedEndPosition())
                        {
                            i++;
                        }

//C# TO C++ CONVERTER TODO TASK: Comparisons involving nullable type instances will need to be rewritten since comparison rules are different between C++ optional and System.Nullable:
                        bool end = i - missedCleavages - 1 >= 0 && oneBasedIndicesToCleaveAfter[i - missedCleavages - 1] + 1 >= proteolysisProduct->getOneBasedBeginPosition() && proteolysisProduct->getOneBasedEndPosition() && OkayLength(std::make_optional(proteolysisProduct->getOneBasedEndPosition().value() - oneBasedIndicesToCleaveAfter[i - missedCleavages - 1] + 1 - 1), std::make_optional(minPeptidesLength), std::make_optional(maxPeptidesLength));
                        if (end)
                        {
//C# TO C++ CONVERTER TODO TASK: C++ does not have an equivalent to the C# 'yield' keyword:
                            yield return new ProteolyticPeptide(protein, oneBasedIndicesToCleaveAfter[i - missedCleavages - 1] + 1, proteolysisProduct->getOneBasedEndPosition().value(), missedCleavages, getCleavageSpecificity()::Full, proteolysisProduct->getType() + " end");
                        }
                    }
                }
            }
        }

        std::vector<ProteolyticPeptide*> Protease::SemiProteolyticDigestion(Protein *protein, InitiatorMethionineBehavior initiatorMethionineBehavior, int maximumMissedCleavages, int minPeptidesLength, int maxPeptidesLength)
        {
            std::vector<ProteolyticPeptide*> intervals;
            std::vector<int> oneBasedIndicesToCleaveAfter = GetDigestionSiteIndices(protein->getBaseSequence());

            // It's possible not to go through this loop (maxMissedCleavages+1>number of indexes), and that's okay. It will get digested in the next loops (finish C/N termini)
            for (int i = 0; i < oneBasedIndicesToCleaveAfter.size() - maximumMissedCleavages - 1; i++)
            {
                bool retain = Retain(i, initiatorMethionineBehavior, protein[0]);
                bool cleave = Cleave(i, initiatorMethionineBehavior, protein[0]);
                int cTerminusProtein = oneBasedIndicesToCleaveAfter[i + maximumMissedCleavages + 1];
                std::unordered_set<int> localOneBasedIndicesToCleaveAfter;
                for (int j = i; j < i + maximumMissedCleavages + 1; j++)
                {
                    localOneBasedIndicesToCleaveAfter.insert(oneBasedIndicesToCleaveAfter[j]);
                }
                if (retain)
                {
                    intervals.AddRange(FixedTermini(oneBasedIndicesToCleaveAfter[i], cTerminusProtein, protein, cleave, retain, minPeptidesLength, maxPeptidesLength, localOneBasedIndicesToCleaveAfter));
                }

                if (cleave)
                {
                    intervals.AddRange(FixedTermini(1, cTerminusProtein, protein, cleave, retain, minPeptidesLength, maxPeptidesLength, localOneBasedIndicesToCleaveAfter));
                }
            }

            // Finish C-term of protein caused by loop being "i < oneBasedIndicesToCleaveAfter.Count - maximumMissedCleavages - 1"
            int last = oneBasedIndicesToCleaveAfter.size() - 1;
            int maxIndexSemi = maximumMissedCleavages < last ? maximumMissedCleavages : last;
            // Fringe C-term peptides
            for (int i = 1; i <= maxIndexSemi; i++)
            {
                // FixedN
                int nTerminusProtein = oneBasedIndicesToCleaveAfter[last - i];
                int cTerminusProtein = oneBasedIndicesToCleaveAfter[last];
                std::unordered_set<int> localOneBasedIndicesToCleaveAfter;
                for (int j = 0; j < i; j++) //include zero, the c terminus
                {
                    localOneBasedIndicesToCleaveAfter.insert(oneBasedIndicesToCleaveAfter[last - j]);
                }
                for (int j = cTerminusProtein; j > nTerminusProtein; j--) //We are hitting the c-terminus here
                {
                    if (OkayLength(std::make_optional(j - nTerminusProtein), std::make_optional(minPeptidesLength), std::make_optional(maxPeptidesLength)))
                    {
                        ProteolyticPeptide tempVar(protein, nTerminusProtein + 1, j, j - nTerminusProtein, getCleavageSpecificity()::Semi, "semi");
                        intervals.push_back(std::find(localOneBasedIndicesToCleaveAfter.begin(), localOneBasedIndicesToCleaveAfter.end(), j) != localOneBasedIndicesToCleaveAfter.end() ? new ProteolyticPeptide(protein, nTerminusProtein + 1, j, j - nTerminusProtein, getCleavageSpecificity()::Full, "full") : &tempVar);
                    }
                }
            }

            // Fringe N-term peptides
            for (int i = 1; i <= maxIndexSemi; i++)
            {
                bool retain = initiatorMethionineBehavior == InitiatorMethionineBehavior::Retain;
                // FixedC
                int nTerminusProtein = retain ? oneBasedIndicesToCleaveAfter[0] : oneBasedIndicesToCleaveAfter[0] + 1; // +1 start after M (since already covered earlier)
                int cTerminusProtein = oneBasedIndicesToCleaveAfter[i];
                std::unordered_set<int> localOneBasedIndicesToCleaveAfter;
                for (int j = 1; j < i; j++) //j starts at 1, because zero is n terminus
                {
                    localOneBasedIndicesToCleaveAfter.insert(oneBasedIndicesToCleaveAfter[j]);
                }
                int start = nTerminusProtein + 1; //plus one to not doublecount the n terminus (in addition to the M term skip)
                for (int j = start; j < cTerminusProtein; j++)
                {
                    if (OkayLength(std::make_optional(cTerminusProtein - j), std::make_optional(minPeptidesLength), std::make_optional(maxPeptidesLength)) && !std::find(localOneBasedIndicesToCleaveAfter.begin(), localOneBasedIndicesToCleaveAfter.end(), j) != localOneBasedIndicesToCleaveAfter.end())
                    {
                        ProteolyticPeptide tempVar2(protein, j + 1, cTerminusProtein, cTerminusProtein - j, getCleavageSpecificity()::Semi, "semi");
                        intervals.push_back(&tempVar2);
                    }
                }
            }

            // Also digest using the proteolysis product start/end indices
            // This should only be things where the proteolysis is not K/R and the
            for (auto proteolysisProduct : protein->getProteolysisProducts())
            {
                if (proteolysisProduct->getOneBasedEndPosition() && proteolysisProduct->getOneBasedBeginPosition() && (proteolysisProduct->getOneBasedBeginPosition() != 1 || proteolysisProduct->getOneBasedEndPosition() != protein->getLength())) //if at least one side is not a terminus
                {
                    int i = 0;
//C# TO C++ CONVERTER TODO TASK: Comparisons involving nullable type instances will need to be rewritten since comparison rules are different between C++ optional and System.Nullable:
                    while (oneBasedIndicesToCleaveAfter[i] < proteolysisProduct->getOneBasedBeginPosition()) //"<" to prevent additions if same index as residues
                    {
                        i++; // Last position in protein is an index to cleave after
                    }

                    // Start peptide
                    for (int j = proteolysisProduct->getOneBasedBeginPosition().value(); j < oneBasedIndicesToCleaveAfter[i]; j++)
                    {
                        if (OkayLength(j - proteolysisProduct->getOneBasedBeginPosition() + 1, std::make_optional(minPeptidesLength), std::make_optional(maxPeptidesLength)))
                        {
                            ProteolyticPeptide tempVar3(protein, proteolysisProduct->getOneBasedBeginPosition().value(), j, j - proteolysisProduct->getOneBasedBeginPosition().value(), getCleavageSpecificity()::Full, proteolysisProduct->getType() + " start");
                            intervals.push_back(&tempVar3);
                        }
                    }
//C# TO C++ CONVERTER TODO TASK: Comparisons involving nullable type instances will need to be rewritten since comparison rules are different between C++ optional and System.Nullable:
                    while (oneBasedIndicesToCleaveAfter[i] < proteolysisProduct->getOneBasedEndPosition()) //"<" to prevent additions if same index as residues, since i-- is below
                    {
                        i++;
                    }

                    // Now that we've obtained an index to cleave after that is past the proteolysis product
                    // we need to backtrack to get the index to cleave that is immediately before the the proteolysis product
                    // to do this, we will do i--
                    // In the nitch case that the proteolysis product is already an index to cleave
                    // no new peptides will be generated using this, so we will forgo i--
                    // this makes peptides of length 0, which are not generated due to the for loop
                    // removing this if statement will result in crashes from c-terminal proteolysis product end positions
                    if (oneBasedIndicesToCleaveAfter[i] != proteolysisProduct->getOneBasedEndPosition())
                    {
                        i--;
                    }

                    // Fin (End)
                    for (int j = oneBasedIndicesToCleaveAfter[i] + 1; j < proteolysisProduct->getOneBasedEndPosition().value(); j++)
                    {
                        if (OkayLength(proteolysisProduct->getOneBasedEndPosition() - j + 1, std::make_optional(minPeptidesLength), std::make_optional(maxPeptidesLength)))
                        {
                            ProteolyticPeptide tempVar4(protein, j, proteolysisProduct->getOneBasedEndPosition().value(), proteolysisProduct->getOneBasedEndPosition().value() - j, getCleavageSpecificity()::Full, proteolysisProduct->getType() + " end");
                            intervals.push_back(&tempVar4);
                        }
                    }
                }
            }
            return intervals;
        }

        std::vector<ProteolyticPeptide*> Protease::FixedTermini(int nTerminusProtein, int cTerminusProtein, Protein *protein, bool cleave, bool retain, int minPeptidesLength, int maxPeptidesLength, std::unordered_set<int> &localOneBasedIndicesToCleaveAfter)
        {
            bool preventMethionineFromBeingDuplicated = nTerminusProtein == 1 && cleave && retain; //prevents duplicate sequences containing N-terminal methionine
            std::vector<ProteolyticPeptide*> intervals;
            if (!preventMethionineFromBeingDuplicated && OkayLength(std::make_optional(cTerminusProtein - nTerminusProtein), std::make_optional(minPeptidesLength), std::make_optional(maxPeptidesLength))) //adds the full length maximum cleavages, no semi
            {
                ProteolyticPeptide tempVar(protein, nTerminusProtein + 1, cTerminusProtein, cTerminusProtein - nTerminusProtein, getCleavageSpecificity()::Full, "full" + (cleave ? ":M cleaved" : ""));
                intervals.push_back(&tempVar); // Maximum sequence length
            }

            // Fixed termini at each internal index
            std::vector<int> internalIndices = Enumerable::Range(nTerminusProtein + 1, cTerminusProtein - nTerminusProtein - 1); //every residue between them, +1 so we don't double count the original full

            std::vector<ProteolyticPeptide*> fixedCTermIntervals;
            if (!preventMethionineFromBeingDuplicated)
            {
                auto indexesOfAcceptableLength = internalIndices.Where([&] (std::any j)
                {
                    OkayLength(cTerminusProtein - j, std::make_optional(minPeptidesLength), std::make_optional(maxPeptidesLength));
                });
                for (auto j : indexesOfAcceptableLength)
                {
                    if (std::find(localOneBasedIndicesToCleaveAfter.begin(), localOneBasedIndicesToCleaveAfter.end(), j) != localOneBasedIndicesToCleaveAfter.end() || (j == 1 && cleave)) //if cleaved on cleavable index or after initiator methionine, record as full
                    {
                        if (j == 1 && cleave) //check we're not doubling it up
                        {
                            ProteolyticPeptide tempVar2(protein, j + 1, cTerminusProtein, cTerminusProtein - j, getCleavageSpecificity()::Full, "full:M cleaved");
                            fixedCTermIntervals.push_back(&tempVar2);
                        }
                        //else //don't allow full unless cleaved, since they're covered by Cterm
                    }
                    else //record it as a semi
                    {
                        ProteolyticPeptide tempVar3(protein, j + 1, cTerminusProtein, cTerminusProtein - j, getCleavageSpecificity()::Semi, "semi" + (cleave ? ":M cleaved" : ""));
                        fixedCTermIntervals.push_back(&tempVar3);
                    }
                }
            }
            std::vector<ProteolyticPeptide*> fixedNTermIntervals = internalIndices.Where([&] (std::any j)
            {
                OkayLength(j - nTerminusProtein, std::make_optional(minPeptidesLength), std::make_optional(maxPeptidesLength));
            })->Select([&] (std::any j)
            {
                std::find(localOneBasedIndicesToCleaveAfter.begin(), localOneBasedIndicesToCleaveAfter.end(), j) != localOneBasedIndicesToCleaveAfter.end() ? new ProteolyticPeptide(protein, nTerminusProtein + 1, j, j - nTerminusProtein, getCleavageSpecificity()::Full, "full" + (cleave ? ":M cleaved" : "")) : new ProteolyticPeptide(protein, nTerminusProtein + 1, j, j - nTerminusProtein, getCleavageSpecificity()::Semi, "semi" + (cleave ? ":M cleaved" : ""));
            });

            return intervals.Concat(fixedCTermIntervals)->Concat(fixedNTermIntervals);
        }

        bool Protease::OkayMinLength(std::optional<int> &peptideLength, std::optional<int> &minPeptidesLength)
        {
//C# TO C++ CONVERTER TODO TASK: Comparisons involving nullable type instances will need to be rewritten since comparison rules are different between C++ optional and System.Nullable:
            return !minPeptidesLength || peptideLength >= minPeptidesLength;
        }

        bool Protease::OkayMaxLength(std::optional<int> &peptideLength, std::optional<int> &maxPeptidesLength)
        {
//C# TO C++ CONVERTER TODO TASK: Comparisons involving nullable type instances will need to be rewritten since comparison rules are different between C++ optional and System.Nullable:
            return !maxPeptidesLength || peptideLength <= maxPeptidesLength;
        }
    }
}