// Copyright (C) 2009-2016 ETH Zurich
// Copyright (C) 2007?-2016 Center for Nanophase Materials Sciences, ORNL
// All rights reserved.
//
// See LICENSE.txt for terms of usage.
// See CITATION.txt for citation guidelines if you use this code for scientific publications.
//
// Author: Urs R. Haehner (haehneru@itp.phys.ethz.ch)
//
// This file tests physics_parameters.hpp
//
// TODO: Add tests for get_buffer_size, pack, unpack and writing.

#include "dca/phys/parameters/physics_parameters.hpp"
#include "gtest/gtest.h"
#include "dca/io/json/json_reader.hpp"

TEST(PhysicsParametersTest, DefaultValues) {
  dca::phys::params::PhysicsParameters pars;

  EXPECT_EQ(1., pars.get_beta());
  EXPECT_FALSE(pars.adjust_chemical_potential());
  EXPECT_EQ(1., pars.get_density());
  EXPECT_EQ(0., pars.get_chemical_potential());
}

TEST(PhysicsParametersTest, ReadAll) {
  dca::io::JSONReader reader;
  dca::phys::params::PhysicsParameters pars;

  reader.open_file(DCA_SOURCE_DIR
                   "/test/unit/phys/parameters/physics_parameters/input_read_all.json");
  pars.readWrite(reader);
  reader.close_file();

  EXPECT_EQ(0.5, pars.get_beta());
  EXPECT_TRUE(pars.adjust_chemical_potential());
  EXPECT_EQ(0.9, pars.get_density());
  EXPECT_EQ(0.14, pars.get_chemical_potential());
}