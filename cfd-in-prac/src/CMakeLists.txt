cmake_minimum_required (VERSION 3.0)
project(CFD_IN_PRAC_test)

set(CMAKE_CXX_STANDARD 14)

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

add_library(geo 
  geo/geometry.cpp 
  geo/readGrid.cpp
  geo/dummyNodes.cpp
  geo/generateEdgelist.cpp  
  geo/deleteTmpElist.cpp  
  geo/computeMetrics.cpp  
  geo/faceVectorsVolumes.cpp
  geo/faceVectorsVolumesBound.cpp  
  geo/checkMetrics.cpp  
  geo/faceVectorsSymm.cpp
  geo/volumeProjections.cpp
  geo/getNumberBoundNodes.cpp
)

add_library(spacedisc
  spacedisc/spaceDiscr.cpp
  spacedisc/dissipInit.cpp 
  spacedisc/dissipRoe1.cpp 
  spacedisc/dissipRoe2.cpp 
  spacedisc/fluxRoe1.cpp 
  spacedisc/fluxRoe2.cpp 
  spacedisc/fluxWalls.cpp 
  spacedisc/gradients.cpp 
  spacedisc/limiterInit.cpp 
  spacedisc/limiter.cpp 
  spacedisc/gradientsVisc.cpp 
  spacedisc/fluxViscous.cpp 
  spacedisc/dissipRoe1Prec.cpp 
  spacedisc/dissipRoe2Prec.cpp  
  spacedisc/limiterRefVals.cpp
)

add_library(solver
  solver/solver.cpp
  solver/initConstants.cpp  
  solver/readSolution.cpp  
  solver/initSolution.cpp
  solver/forces.cpp
  solver/massFlow.cpp
  solver/convergence.cpp
  solver/writeSolution.cpp 
)

add_executable(main 
  main.cpp
  fluidProps.cpp
  timeDiscr.cpp
  userInput.cpp  
  output.cpp  
  dependentVars.cpp 
  outputConvergence.cpp
  outputSurfaces.cpp
  outputFlowfield.cpp
  solve.cpp
  densityChange.cpp  
  timeStep.cpp 
  irsmoo.cpp 
  )

  target_link_libraries(main bcond geo spacedisc solver)

  set(CMAKE_CXX_FLAGS "-g")