/*!
 * \file   mfront/src/KinematicHardeningRuleFactory.cxx
 * \brief
 * \author Thomas Helfer
 * \date   15/03/2018
 * \copyright Copyright (C) 2006-2018 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence
 * or the CECILL-A licence. A copy of thoses licences are delivered
 * with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include "TFEL/Raise.hxx"
#include "MFront/BehaviourBrick/PragerKinematicHardeningRule.hxx"
#include "MFront/BehaviourBrick/OhnoWangKinematicHardeningRule.hxx"
#include "MFront/BehaviourBrick/Chaboche1991KinematicHardeningRule.hxx"
#include "MFront/BehaviourBrick/BurletCailletaudKinematicHardeningRule.hxx"
#include "MFront/BehaviourBrick/ArmstrongFrederickKinematicHardeningRule.hxx"
#include "MFront/BehaviourBrick/KinematicHardeningRuleFactory.hxx"

namespace mfront {

  namespace bbrick {

    KinematicHardeningRuleFactory& KinematicHardeningRuleFactory::getFactory() {
      static KinematicHardeningRuleFactory i;
      return i;
    }  // end of KinematicHardeningRuleFactory::getFactory

    void KinematicHardeningRuleFactory::addGenerator(const std::string& n,
                                                     const Generator& g) {
      if (!this->generators.insert({n, g}).second) {
        tfel::raise(
            "KinematicHardeningRuleFactory::addGenerator: "
            "generator '" +
            n + "' already registred");
      }
    }  // end of KinematicHardeningRuleFactory::addGenerator

    std::shared_ptr<KinematicHardeningRule> KinematicHardeningRuleFactory::generate(
        const std::string& n) const {
      const auto p = this->generators.find(n);
      if (p == this->generators.end()) {
        tfel::raise(
            "KinematicHardeningRuleFactory::generate: "
            "no generator named '" +
            n + "'");
      }
      return p->second();
    }  // end of KinematicHardeningRuleFactory::generate

    KinematicHardeningRuleFactory::KinematicHardeningRuleFactory() {
      this->addGenerator("Prager", []() {
        return std::make_shared<bbrick::PragerKinematicHardeningRule>();
      });
      this->addGenerator("ArmstrongFrederick", []() {
        return std::make_shared<
            bbrick::ArmstrongFrederickKinematicHardeningRule>();
      });
      this->addGenerator("Armstrong-Frederick", []() {
        return std::make_shared<
            bbrick::ArmstrongFrederickKinematicHardeningRule>();
      });
      this->addGenerator("BurletCailletaud", []() {
        return std::make_shared<
            bbrick::BurletCailletaudKinematicHardeningRule>();
      });
      this->addGenerator("Burlet-Cailletaud", []() {
        return std::make_shared<
            bbrick::BurletCailletaudKinematicHardeningRule>();
      });
      this->addGenerator("BurletCailletaud1987", []() {
        return std::make_shared<
            bbrick::BurletCailletaudKinematicHardeningRule>();
      });
      this->addGenerator("Burlet-Cailletaud 1987", []() {
        return std::make_shared<
            bbrick::BurletCailletaudKinematicHardeningRule>();
      });
      this->addGenerator("Chaboche1991", []() {
        return std::make_shared<
            bbrick::Chaboche1991KinematicHardeningRule>();
      });
      this->addGenerator("Chaboche 1991", []() {
        return std::make_shared<
            bbrick::Chaboche1991KinematicHardeningRule>();
      });
    }  // end of KinematicHardeningRuleFactory::KinematicHardeningRuleFactory

    KinematicHardeningRuleFactory::~KinematicHardeningRuleFactory() = default;

  }  // end of namespace bbrick

}  // end of namespace mfront
