# snippet to find the closest city tile to a position
def keeping_research(player):
    # the cities are stored as a dictionary mapping city id to the city object, which has a citytiles field that
    # contains the information of all citytiles in that city
    actions = []

    for k, city in player.cities.items():
        for city_tile in city.citytiles:
            if city_tile.can_act(): 
                actions.append(city_tile.research())

    return actions