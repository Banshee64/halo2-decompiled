from ready import by_file, components, likely_objects, line, ready


def row(va, calls='', owner='game', status='todo', size=10, name='', object=''):
    return dict(va=f'{va:08x}', size=str(size), owner=owner, status=status, name=name, object=object,
                calls=' '.join(f'{c:08x}' for c in calls))


def test_components_groups_mutual_recursion():
    groups = components({1: {2}, 2: {1}, 3: {1}})
    assert {frozenset(g) for g in groups} == {frozenset({1, 2}), frozenset({3})}


def test_ready_needs_matched_or_library_callees():
    rows = dict([
        (0x10, row(0x10, calls=[0x20])),                       # calls a todo game function
        (0x20, row(0x20, size=5)),                             # leaf
        (0x30, row(0x30, calls=[0x40], size=7)),               # calls a library function
        (0x40, row(0x40, owner='xdk:libcmt')),
        (0x50, row(0x50, calls=[0x60], size=3)),
        (0x60, row(0x60, status='matched')),
        (0x70, row(0x70, status='matched')),                   # done already
    ])
    assert [r['va'] for r in ready(rows)] == ['00000050', '00000020', '00000030']


def test_ready_treats_recursion_as_one_unit():
    rows = {0x10: row(0x10, calls=[0x20]), 0x20: row(0x20, calls=[0x10])}
    assert {r['va'] for r in ready(rows)} == {'00000010', '00000020'}


def _scene():
    return dict([
        (0x10, row(0x10, object='a.obj', status='matched')),
        (0x20, row(0x20, size=9)),                      # no object: after a.obj
        (0x30, row(0x30, size=3, object='b.obj')),
        (0x40, row(0x40, size=5)),                      # after b.obj
        (0x50, row(0x50, size=4, object='a.obj')),
    ])


def test_likely_object_is_own_or_nearest_preceding():
    rows = _scene()
    objects = likely_objects(rows, ready(rows))
    assert objects == {0x20: '~a.obj', 0x30: 'b.obj', 0x40: '~b.obj', 0x50: 'a.obj'}


def test_likely_object_stops_at_a_section_start():
    rows = _scene()
    assert likely_objects(rows, ready(rows), boundaries=[0x18])[0x20] == ''


def test_ready_line_has_object():
    rows = _scene()
    objects = likely_objects(rows, ready(rows))
    assert line(rows[0x20], objects) == '00000020 9 ~a.obj - -'


def test_by_file_groups_by_smallest_member_then_size():
    rows = _scene()
    objects = likely_objects(rows, ready(rows))
    groups = by_file(ready(rows), objects)
    assert [(o, [r['va'] for r in g]) for o, g in groups] == [
        ('b.obj', ['00000030', '00000040']), ('a.obj', ['00000050', '00000020'])]
