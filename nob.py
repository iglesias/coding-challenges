import nobuild.nob as nob

nob.make_and_run_cmd(["ls", "--color", "-l", "-t"])
nob.make_and_run_cmd(["pwd"])

nob.init_specifics()
specifics = nob.specifics()
for file in specifics.keys():
    print(f'{file}:')
    for cmd in specifics[file]:
        print(f'  {cmd.line}')
