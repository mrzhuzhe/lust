#import math, sys
import time
from lux.game import Game
from lux.game_map import Cell, RESOURCE_TYPES
from lux.constants import Constants
from lux.game_constants import GAME_CONSTANTS
from lux import annotate

# custom s
from mytest.research import keeping_research
from mytest.finder.convexhull import getPolynomialHull
# custom e

DIRECTIONS = Constants.DIRECTIONS
game_state = None

import builtins as __builtin__
from lux.game import Game, Missions

from make_actions import make_city_actions, make_unit_missions, make_unit_actions
from make_annotations import annotate_game_state, annotate_missions, annotate_movements

game_state = Game()
missions = Missions()

def game_logic(game_state: Game, missions: Missions, DEBUG=False):
    if DEBUG: print = __builtin__.print
    else: print = lambda *args: None

    game_state.calculate_features(missions)
    state_annotations = annotate_game_state(game_state)
    actions_by_cities = make_city_actions(game_state, missions, DEBUG=DEBUG)
    missions = make_unit_missions(game_state, missions, DEBUG=DEBUG)
    mission_annotations = annotate_missions(game_state, missions)
    missions, actions_by_units = make_unit_actions(game_state, missions, DEBUG=DEBUG)
    movement_annotations = annotate_movements(game_state, actions_by_units)

    print("actions_by_cities", actions_by_cities)
    print("actions_by_units", actions_by_units)
    print("mission_annotations", mission_annotations)
    print("movement_annotations", movement_annotations)
    actions = actions_by_cities + actions_by_units + mission_annotations + movement_annotations + state_annotations
    return actions, game_state, missions


def agent(observation, configuration):
    global game_state, missions
    
    ### Do not edit ###
    if observation["step"] == 0:
        game_state = Game()
        game_state._initialize(observation["updates"])
        game_state._update(observation["updates"][2:])
        game_state.id = observation.player
        game_state.fix_iteration_order()
    else:
        game_state._update(observation["updates"])
    
    game_state.compute_start_time = time.time()
    actions, game_state, missions = game_logic(game_state, missions)

    return actions
