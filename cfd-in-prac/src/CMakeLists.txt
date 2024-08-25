cmake_minimum_required (VERSION 3.0)
project(CFD_IN_PRAC_test)

set( target_name main)
set( CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin) 
set( CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib )

include_directories(.)

add_library(bcond 
  bcond/bndConds.cpp
  bcond/bcondInflow.cpp
  bcond/bcondOutflow.cpp
  bcond/bcondFarfield.cpp
  bcond/bcondWallVisc.cpp
  bcond/boundaryConditions.cpp
  bcond/zeroResiduals.cpp 
  bcond/periodic.cpp)

add_executable(main 
  main.cpp 
  geometry.cpp 
  fluidProps.cpp
  spaceDiscr.cpp
  timeDiscr.cpp
  userInput.cpp
  solver.cpp
  output.cpp
  initConstants.cpp
  readGrid.cpp
  dummyNodes.cpp
  generateEdgelist.cpp
  deleteTmpElist.cpp
  computeMetrics.cpp
  faceVectorsVolumes.cpp
  faceVectorsVolumesBound.cpp
  checkMetrics.cpp
  faceVectorsSymm.cpp
  volumeProjections.cpp
  getNumberBoundNodes.cpp  
  readSolution.cpp
  initSolution.cpp
  dependentVars.cpp
  writeSolution.cpp  
  limiterRefVals.cpp
  outputConvergence.cpp
  outputSurfaces.cpp
  outputFlowfield.cpp
  solve.cpp
  densityChange.cpp
  forces.cpp
  massFlow.cpp
  convergence.cpp 
  dissipInit.cpp 
  timeStep.cpp 
  dissipRoe1.cpp 
  dissipRoe2.cpp 
  fluxRoe1.cpp 
  fluxRoe2.cpp 
  fluxWalls.cpp 
  irsmoo.cpp 
  gradients.cpp 
  limiterInit.cpp 
  limiter.cpp 
  gradientsVisc.cpp 
  fluxViscous.cpp 
  dissipRoe1Prec.cpp 
  dissipRoe2Prec.cpp)

  target_link_libraries(main bcond)