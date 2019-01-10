#!/usr/bin/env python

import os
import sys
import argparse
import glob
import subprocess

def parse_args():
  parser = argparse.ArgumentParser(description='Check whether the program outputs correctly')
  parser.add_argument('target', metavar='target-program', type=str, help='program to check')
  parser.add_argument('--ans', '-a', metavar='answer', type=str, help='answer program (default: bin/dp.out)', default='bin/dp.out')
  return parser.parse_args()

def run_command(cmd):
  p = subprocess.Popen(' '.join(cmd), shell=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
  stdout_data, stderr_data = p.communicate()
  return p.returncode, stdout_data, stderr_data

def main():
  arg = parse_args()
  if not os.path.exists(arg.ans):
    sys.stderr.write("{} does not exist\n".format(arg.ans))
    exit(1)
  if not os.path.exists(arg.target):
    sys.stderr.write("{} does not exist\n".format(arg.target))
    exit(1)
  testcases = [
    *glob.glob("tests/n_6/*"),
    *glob.glob("tests/n_8/*"),
    *glob.glob("tests/n_10/*"),
    *glob.glob("tests/n_12/*"),
    *glob.glob("tests/n_14/*"),
    *glob.glob("tests/n_16/*"),
  ]
  ok_count = 0
  ng_count = 0
  for testcase in testcases:
    sys.stdout.write("testing {}...".format(testcase))
    answer_status, answer_output, answer_error = run_command([arg.ans, testcase])
    if answer_status != 0:
      print(" ng")
      sys.stderr.write("Reason: answer program exited abnormally: exit code {}\n".format(answer_status))
      sys.stderr.write("stdout: {}".format(answer_output.decode('utf-8')))
      sys.stderr.write("stderr: {}".format(answer_error.decode('utf-8')))
      continue
    target_status, target_output, target_error = run_command([arg.target, testcase])
    if target_status != 0:
      print(" ng")
      sys.stderr.write("Reason: target program exited abnormally: exit code {}\n".format(target_status))
      sys.stderr.write("stdout: {}".format(target_output.decode('utf-8')))
      sys.stderr.write("stderr: {}".format(target_error.decode('utf-8')))
      continue
    if answer_output != target_output:
      print(" ng")
      sys.stderr.write("{} outputs {} but {} outputs {}\n".format(
        arg.ans, answer_output.decode('utf-8'), arg.target, target_output.decode('utf-8')
      ))
      ng_count += 1
      continue
    print(" ok")
    ok_count += 1
  print("===============================================")
  print("ok: {}, ng: {}".format(ok_count, ng_count))
  if ng_count > 0:
    exit(1)

main()
