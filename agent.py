import math, sys
from lux.game import Game
from lux.game_map import Cell, RESOURCE_TYPES
from lux.constants import Constants
from lux.game_constants import GAME_CONSTANTS
from lux import annotate

# custom s
from agents.research import keeping_research
from agents.finder.convexhull import getPolynomialHull
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
    global game_state

    ### Do not edit ###
    if observation["step"] == 0:
        game_state = Game()
        game_state._initialize(observation["updates"])
        game_state._update(observation["updates"][2:])
        game_state.id = observation.player
    else:
        game_state._update(observation["updates"])
    
    actions = []

    ### AI Code goes down here! ### 
    player = game_state.players[observation.player]
    opponent = game_state.players[(observation.player + 1) % 2]
    width, height = game_state.map.width, game_state.map.height

    resource_tiles: list[Cell] = []

    for y in range(height):
        for x in range(width):
            cell = game_state.map.get_cell(x, y)
            if cell.has_resource():
                resource_tiles.append(cell)
                # mark resource_tile position
                #actions.append(annotate.circle(cell.pos.x, cell.pos.y))
                actions.append(annotate.text(cell.pos.x, cell.pos.y, cell.resource.type, 16))    # no use

    #   mark convex hull
    actions += getPolynomialHull(resource_tiles, width, height)

    # we iterate over all our units and do something with them
    for unit in player.units:
        if unit.is_worker() and unit.can_act():
            actions.append(unit.move(DIRECTIONS.EAST))       
            """
            closest_dist = math.inf
            closest_resource_tile = None
            if unit.get_cargo_space_left() > 0:
                # if the unit is a worker and we have space in cargo, lets find the nearest resource tile and try to mine it
                for resource_tile in resource_tiles:
                    if resource_tile.resource.type == Constants.RESOURCE_TYPES.COAL and not player.researched_coal(): continue
                    if resource_tile.resource.type == Constants.RESOURCE_TYPES.URANIUM and not player.researched_uranium(): continue
                    dist = resource_tile.pos.distance_to(unit.pos)
                    if dist < closest_dist:
                        closest_dist = dist
                        closest_resource_tile = resource_tile
                if closest_resource_tile is not None:
                    actions.append(unit.move(unit.pos.direction_to(closest_resource_tile.pos)))
            else:
                # if unit is a worker and there is no cargo space left, and we have cities, lets return to them
                if len(player.cities) > 0:
                    closest_dist = math.inf
                    closest_city_tile = None
                    for k, city in player.cities.items():
                        for city_tile in city.citytiles:
                            dist = city_tile.pos.distance_to(unit.pos)
                            if dist < closest_dist:
                                closest_dist = dist
                                closest_city_tile = city_tile
                    if closest_city_tile is not None:
                        move_dir = unit.pos.direction_to(closest_city_tile.pos)
                        actions.append(unit.move(move_dir))
            """
            

    # ALL city_tiles doing research
    if player.research_points < 200:        
        actions += keeping_research(player)
    # you can add debug annotations using the functions in the annotate object
    # actions.append(annotate.circle(0, 0))

    return actions
