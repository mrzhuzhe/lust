# complexity 4n memory useage n + 4n
from lux import annotate

def getPolynomialHull(resources, width, height):
    dictArr = []
    for cell in resources:
        dictArr.append(str(cell.pos.x) + "-" + str(cell.pos.y))

    actions = []
    for cell in resources:       
        x = cell.pos.x
        y = cell.pos.y
        l = x - 1
        r = x + 1
        t = y + 1
        d = y - 1

        if l >= 0 and not str(l) + "-" + str(y) in dictArr:
            actions.append(annotate.x(l, y))
        if r < width and not str(r) + "-" + str(y) in dictArr:
            actions.append(annotate.x(r, y))
        if t < height and not str(x) + "-" + str(t) in dictArr:
            actions.append(annotate.x(x, t))
        if d >= 0 and not str(x) + "-" + str(d) in dictArr:
            actions.append(annotate.x(x, d))
        #actions.append(annotate.sidetext(str(cell.pos.x) + str(cell.pos.y)))

    return actions